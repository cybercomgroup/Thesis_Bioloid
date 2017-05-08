#include "audio.h"
#include <unistd.h>
#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include "pocketsphinx.h"

#ifdef __arm__
#define MODELDIR "/home/pi/Downloads/sphinx/pocketsphinx-5prealpha/model"
#define MIC "hw:0,0"
#else
#define MODELDIR "/home/tobbeh/Downloads/sphinx/pocketsphinx-5prealpha/model"
#define MIC NULL
#endif

ps_decoder_t *ps;
cmd_ln_t *config;
char const *hyp, *uttid;
string commands[30];
int16 buf[512];
int rv, commandSize=-1;
int32 score;

string audio_parseCommand(string s)
{
  //string command;
  int pos = s.find_first_of(" \t");
  if(pos != -1)
  {
    if(s.substr(0,pos).compare("ROBOT") == 0)
    {
      return s.substr(pos);
    }
  }
	return "";
}


int audio_init(string lm, string dict)
{
	err_set_logfp(NULL);
	config = cmd_ln_init(NULL, ps_args(), TRUE,
	"-hmm", MODELDIR "/en-us/en-us",
	"-lm", lm.c_str(),
	"-dict", dict.c_str(),
	NULL);
	if (config == NULL) {
		fprintf(stderr, "Failed to create config object, see log for details\n");
		return -1;
	}

	ps = ps_init(config);
	if (ps == NULL) {
		fprintf(stderr, "Failed to create recognizer, see log for details\n");
		return -1;
	}

	return 0;
}

int audio_getCommandsSize()
{
	return commandSize;
}

string audio_popCommand()
{
	return commands[commandSize--];
}

int audio_listenForCommand()
{
	ad_rec_t *ad;
	int16 adbuf[2048];
	uint8 utt_started, in_speech;
	int32 k;
	string hyp = "";

	if ((ad = ad_open_dev(MIC,
												(int) cmd_ln_float32_r(config,
																							 "-samprate"))) == NULL)
			E_FATAL("Failed to open audio device\n");
	if (ad_start_rec(ad) < 0)
			E_FATAL("Failed to start recording\n");

	if (ps_start_utt(ps) < 0)
			E_FATAL("Failed to start utterance\n");
	utt_started = FALSE;
	E_INFO("Ready....\n");

	for (;;) {
			if ((k = ad_read(ad, adbuf, 2048)) < 0)
					E_FATAL("Failed to read audio\n");
			ps_process_raw(ps, adbuf, k, FALSE, FALSE);
			in_speech = ps_get_in_speech(ps);
			if (in_speech && !utt_started) {
					utt_started = TRUE;
					E_INFO("Listening...\n");
			}
			if (!in_speech && utt_started) {
					/* speech -> silence transition, time to start new utterance  */
					ps_end_utt(ps);
					hyp = ps_get_hyp(ps, NULL );
					if (hyp != "") {
							if(commandSize < 30){
								commandSize++;
								commands[commandSize] = hyp;
							}
							hyp="";
							/*
							printf("%s\n", hyp);
							fflush(stdout);*/
					}
					if (ps_start_utt(ps) < 0)
							E_FATAL("Failed to start utterance\n");
					utt_started = FALSE;
					E_INFO("Ready....\n");
			}
			usleep(100);
	}
	ad_close(ad);
}

void audio_listen()
{
    ad_rec_t *ad;
    int16 adbuf[2048];
    uint8 utt_started, in_speech;
    int32 k;
    char const *hyp;

    if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"),
                          (int) cmd_ln_float32_r(config,
                                                 "-samprate"))) == NULL)
        E_FATAL("Failed to open audio device\n");
    if (ad_start_rec(ad) < 0)
        E_FATAL("Failed to start recording\n");

    if (ps_start_utt(ps) < 0)
        E_FATAL("Failed to start utterance\n");
    utt_started = FALSE;
    E_INFO("Ready....\n");

    for (;;) {
        if ((k = ad_read(ad, adbuf, 2048)) < 0)
            E_FATAL("Failed to read audio\n");
        ps_process_raw(ps, adbuf, k, FALSE, FALSE);
        in_speech = ps_get_in_speech(ps);
        if (in_speech && !utt_started) {
            utt_started = TRUE;
            E_INFO("Listening...\n");
        }
        if (!in_speech && utt_started) {
            /* speech -> silence transition, time to start new utterance  */
            ps_end_utt(ps);
            hyp = ps_get_hyp(ps, NULL );
            if (hyp != NULL) {
                printf("%s\n", hyp);
                fflush(stdout);
            }

            if (ps_start_utt(ps) < 0)
                E_FATAL("Failed to start utterance\n");
            utt_started = FALSE;
            E_INFO("Ready....\n");
        }
        usleep(100);
    }
    ad_close(ad);
}


int audio_interpetFile(FILE *fh)
{
	rv = ps_start_utt(ps);

	while (!feof(fh)) {
		size_t nsamp;
		nsamp = fread(buf, 2, 512, fh);
		rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
	}

	rv = ps_end_utt(ps);
	hyp = ps_get_hyp(ps, &score);
	printf("Recognized: %s\n", hyp);

	fclose(fh);
	ps_free(ps);
	cmd_ln_free_r(config);
}
