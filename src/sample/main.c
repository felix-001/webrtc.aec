#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webrtc/modules/audio_processing/aec/include/echo_cancellation.h"
#include "webrtc/common_audio/signal_processing/include/signal_processing_library.h"

#define  NN 160

int WebRtcAecTest()
{
    short far_frame[NN], near_frame[NN], out_frame[NN];
    void *aecmInst = NULL;
    FILE *fp_far  = fopen("../media/speaker.pcm", "rb");
    FILE *fp_near = fopen("../media/micin.pcm", "rb");
    FILE *fp_out  = fopen("out.pcm", "wb");
    AecConfig config;

    if (!fp_far) {
        printf("open ../media/speaker.pcm error\n");
        return -1;
    }
    if (!fp_near) {
        fclose(fp_far);
        printf("open ../media/micin.pcm error\n");
        return -1;
    }

    WebRtcAec_Create(&aecmInst);
    WebRtcAec_Init(aecmInst, 8000, 8000);
    config.nlpMode = kAecNlpConservative;
    WebRtcAec_set_config(aecmInst, config);
    for(;;) {
        if (NN == fread(far_frame, sizeof(short), NN, fp_far)) {
            fread(near_frame, sizeof(short), NN, fp_near);
            WebRtcAec_BufferFarend(aecmInst, far_frame, NN);//对参考声音(回声)的处理
            WebRtcAec_Process(aecmInst, near_frame, NULL, out_frame, NULL, NN, 40, 0);//回声消除
            fwrite(out_frame, sizeof(short), NN, fp_out);
        } else {
            break;
        }
    }

    fclose(fp_far);
    fclose(fp_near);
    fclose(fp_out);
    WebRtcAec_Free(aecmInst);
    return 0;
}

int main()
{
    for(;;) {
        WebRtcAecTest();
    }
	return 0;
}

