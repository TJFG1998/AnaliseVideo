
#define _CRT_SECURE_NO_DEPRECATE
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <stdio.h>

extern "C" {
#include "vc.h"
}


int main(void)
{
	char *videofile;
	int escolha = 0;
	while (escolha == 0) {
		printf("Video: (1-4)\n");
		scanf("%d", &escolha);
		if (escolha == 1) { videofile = "video1_tp2.mp4"; }
		else if(escolha==2){ videofile = "video2-tp2.mp4"; }
		else if(escolha==3){ videofile = "video3-tp2.mp4"; }
		else if(escolha==4){ videofile = "video4-tp2.mp4"; }
		else { escolha = 0; }
	}

	system("cls");
	// Video
	
	CvCapture *capture;
	IplImage *frame;
	IplImage *frame2;
	
	struct
	{
		int width, height;
		int ntotalframes;
		int fps;
		int nframe;
	} video;
	// Texto
	CvFont font, fontbkg;
	double hScale = 1.0;
	double vScale = 1.0;
	int lineWidth = 1;
	char str[500] = { 0 };
	// Outros
	int key = 0;

	/* Leitura de video de um ficheiro */
	/* NOTA IMPORTANTE:
	O ficheiro video-tp2.avi devera estar localizado no mesmo directorio que o ficheiro de codigo fonte.
	*/
	capture = cvCaptureFromFile(videofile);

	/* Verifica se foi possivel abrir o ficheiro de video */
	if (!capture)
	{
		fprintf(stderr, "Erro ao abrir o ficheiro de video!\n");
		return 1;
	}

	/* N�mero total de frames no v�deo */
	video.ntotalframes = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	/* Frame rate do v�deo */
	video.fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	/* Resolu��o do v�deo */
	video.width = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	video.height = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);

	/* Cria uma janela para exibir o v�deo */
	cvNamedWindow("VC - TP2", CV_WINDOW_AUTOSIZE);

	/* Inicializa a fonte */
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, hScale, vScale, 0, lineWidth);
	cvInitFont(&fontbkg, CV_FONT_HERSHEY_SIMPLEX, hScale, vScale, 0, lineWidth + 1);


	int *excluir=(int *)calloc(10,sizeof(int));
	int *total = (int *)calloc(9,sizeof(int));


	while (key != 'q') {
		/* Leitura de uma frame do v�deo */
		frame = cvQueryFrame(capture);
		frame2 = cvQueryFrame(capture);
		/* Verifica se conseguiu ler a frame */
		if (!frame) break;

		cvMorphologyEx(frame, frame, NULL, cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL), CV_MOP_OPEN, 5);
		cvMorphologyEx(frame2, frame2, NULL, cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL), CV_MOP_OPEN, 5);
		/* N�mero da frame a processar */
		video.nframe = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
		if (escolha == 4 && video.nframe > 19 && video.nframe < 215) {
			ProcessaFrame(frame, frame2, excluir, total);
		}
		else if (escolha == 4 && video.nframe >= 214) {
			break;
		}
		else if (escolha == 3 && video.nframe > 19 && video.nframe < 417) {
			ProcessaFrame(frame, frame2, excluir, total);
		}
		else if (escolha == 3 && video.nframe >= 416) {
			break;
		}
		else if(video.nframe > 19) {
			ProcessaFrame(frame, frame2,excluir,total);
		}

		/* Exemplo de inser��o texto na frame */
		sprintf(str, "RESOLUCAO: %dx%d", video.width, video.height);
		cvPutText(frame, str, cvPoint(20, 25), &fontbkg, cvScalar(0, 0, 0));
		cvPutText(frame, str, cvPoint(20, 25), &font, cvScalar(255, 255, 255));
		sprintf(str, "TOTAL DE FRAMES: %d", video.ntotalframes);
		cvPutText(frame, str, cvPoint(20, 50), &fontbkg, cvScalar(0, 0, 0));
		cvPutText(frame, str, cvPoint(20, 50), &font, cvScalar(255, 255, 255));
		sprintf(str, "FRAME RATE: %d", video.fps);
		cvPutText(frame, str, cvPoint(20, 75), &fontbkg, cvScalar(0, 0, 0));
		cvPutText(frame, str, cvPoint(20, 75), &font, cvScalar(255, 255, 255));
		sprintf(str, "N. FRAME: %d", video.nframe);
		cvPutText(frame, str, cvPoint(20, 100), &fontbkg, cvScalar(0, 0, 0));
		cvPutText(frame, str, cvPoint(20, 100), &font, cvScalar(255, 255, 255));

		

		/* Exibe a frame */
		cvShowImage("VC - TP2", frame);

		/* Sai da aplica��o, se o utilizador premir a tecla 'q' */
		key = cvWaitKey(1);
	}

	total[8] = total[0] + total[1] + total[2] + total[3] + total[4] + total[5] + total[6] + total[7];
	float tot = total[0] * 0.01 + total[1] * 0.02 + total[2] * 0.05 + total[3] * 0.1 + total[4] * 0.2 + total[5] * 0.5 + total[6] + total[7] * 2;
	printf("Total de moedas:%d\nTotal de cada moeda:", total[8]);
	printf("1 centimo:%d\n", total[0]);
	printf("2 centimos:%d\n", total[1]);
	printf("5 centimos:%d\n", total[2]);
	printf("10 centimos:%d\n", total[3]);
	printf("20 centimos:%d\n", total[4]);
	printf("50 centimos:%d\n", total[5]);
	printf("1 euro:%d\n", total[6]);
	printf("2 euros:%d\n", total[7]);
	printf("Valor total:%f euros\n", tot);
	system("pause");
	/* Fecha a janela */
	cvDestroyWindow("VC - TP2");

	/* Fecha o ficheiro de v�deo */
	cvReleaseCapture(&capture);

	


	return 0;
}