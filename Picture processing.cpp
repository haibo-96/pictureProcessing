#include<iostream>
#include<windows.h>
#include<malloc.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

using namespace std;

int pdqb(unsigned short x)                              //�ж�
{
    if(x<128)
        return x;
    else
        return x+256;
}

int write_txt(int width, int height, char *rgb)
{
    int i, j, k, x, ipos;
    int bytesPerLine;
    unsigned char *line;
    char filename[]="f://dmt//fuben.txt";
    FILE *file;

    bytesPerLine = 3*width;
    x=((3 * (width + 1) / 4) * 4)-3*width;                  //ÿ��Ӧ�ò����ֽ���
    k=0;

    file = fopen (filename, "wb");
    if (file == NULL) return(0);

    line = (unsigned char*)malloc(bytesPerLine);
    if (line == NULL)
    {
        fprintf(stderr, "Can't allocate memory for BMP file.\n");
        return(0);
    }

    for (i = 0; i <= height; i++)
    {
        for (j = 0; j < width; j++)
        {
            ipos = 3 * (width * i + j) + k;                 //ͬwrite_bmp
            line[3*j] = rgb[ipos];
            line[3*j+1] = rgb[ipos + 1];
            line[3*j+2] = rgb[ipos + 2];
        }
        fwrite(line, bytesPerLine, 1, file);
        k = k + x;                                          //����Ӧ���������ֽ���
    }

    free(line);
    fclose(file);

    return(1);
}

int write_bmp(const char *filename, int width, int height, char *rgb)   //д��bmp�ļ�
{
    int i, j, ipos;
    int bytesPerLine;
    unsigned char *line;

    FILE *file;
    BITMAPFILEHEADER bf;                      //ͼ���ļ�ͷ
    BITMAPINFOHEADER bi;                      //ͼ���ļ�ͷ��Ϣ

    /* The length of each line must be a multiple of 4 bytes */

    bytesPerLine = (3 * (width + 1) / 4) * 4; //����ÿ�е��ֽ���
    bf.bfType=0x4d42;                        //�����ļ�ͷ��Ϣ
    bf.bfOffBits = 54;
    bf.bfSize = bf.bfOffBits + bytesPerLine * height;
    bf.bfReserved1=bf.bfReserved2 = 0;
    bi.biSize = 40;
    bi.biWidth = width;
    bi.biHeight = height;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = 0;
    bi.biSizeImage = bytesPerLine * height;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    file = fopen (filename, "wb");
    if (file == NULL) return(0);

    fwrite(&bf.bfType, 2, 1, file);                         //д���ļ�ͷ��Ϣ
    fwrite(&bf.bfSize, 4, 1, file);
    fwrite(&bf.bfReserved1, 2, 1, file);
    fwrite(&bf.bfReserved2, 2, 1, file);
    fwrite(&bf.bfOffBits, 4, 1, file);
    fwrite(&bi.biSize, 4, 1, file);
    fwrite(&bi.biWidth, 4, 1, file);
    fwrite(&bi.biHeight, 4, 1, file);
    fwrite(&bi.biPlanes, 2, 1, file);
    fwrite(&bi.biBitCount, 2, 1, file);
    fwrite(&bi.biCompression, 4, 1, file);
    fwrite(&bi.biSizeImage, 4, 1, file);
    fwrite(&bi.biXPelsPerMeter, 4, 1, file);
    fwrite(&bi.biYPelsPerMeter, 4, 1, file);
    fwrite(&bi.biClrUsed, 4, 1, file);
    fwrite(&bi.biClrImportant, 4, 1, file);

    line = (unsigned char*)malloc(bytesPerLine);
    if (line == NULL)
    {
        fprintf(stderr, "Can't allocate memory for BMP file.\n");
        return(0);
    }

    for (i = 0; i <= height; i++)
    {
        for (j = 0; j < width; j++)
        {
            ipos = 3 * (width * i + j);                     //�ҵ���ǰ���ص��λ��
            line[3*j] = rgb[ipos];                          //����ǰ���RGB����д��line��
            line[3*j+1] = rgb[ipos + 1];
            line[3*j+2] = rgb[ipos + 2];
        }
        fwrite(line, bytesPerLine, 1, file);                //��һ��RGBֵд���ļ���
    }

    free(line);
    fclose(file);

    return(1);
}

void save(int w,int h,char* buf)                            //�����ļ�
{
    char fname[30];
    cout<<"������Ҫ�����ļ������֣�";
    cin>>fname;
    write_bmp(fname,w,h,buf);
}

void rgbTohd(int i,char *rgb)                               //�Ҷ�ת��
{
     unsigned short r,g,b,gary;

     for(int j=0;j<=i;j++)
     {
         b=pdqb((unsigned short)(*rgb));                    //��ȡRGBֵ
         g=pdqb((unsigned short)(*(rgb+1)));
         r=pdqb((unsigned short)(*(rgb+2)));
         gary=(299*r+587*g+114*b+500)/1000;                 //��ʽ����Gary
         (*rgb)=(*(rgb+1))=(*(rgb+2))=gary;                 //���¸�ֵ��RGB
         rgb=rgb+3;
     }

}

void rgbTowb(int i,char* rgb)                               //��ֵͼת��
{
     rgbTohd(i,rgb);                                        //�Ƚ�ͼƬתΪ�Ҷ�

     for(int j=0;j<=i;j++)
     {
         if((*rgb)<=50)                                     //���ó���Ϊ50
            (*rgb)=(*(rgb+1))=(*(rgb+2))=0;                 //С��50��ֵΪ0
         else
            (*rgb)=(*(rgb+1))=(*(rgb+2))=255;               //����50��ֵΪ255
         rgb=rgb+3;
     }
}

void contrastRatiochange(char* pImgData,int height,int width,int lineByte,int ChangeVale)   //�Աȶȵ���
{
    for(int i=0;i<height;i++)
        {
            for(int j=0;j<3*width;j=j+3)                    //��ȡRGBֵ
            {   int b=pdqb(*(pImgData+i*lineByte+j));
                int g=pdqb(*(pImgData+i*lineByte+j+1));
                int r=pdqb(*(pImgData+i*lineByte+j+2));
                if(b>=70)                                   //�����ж�RGB�볣���Ĺ�ϵ����������û�����������С�����ȥ�û���������
                    b+=ChangeVale;
                else
                    b-=ChangeVale;
                if(g>=70)
                    g+=ChangeVale;
                else
                    g-=ChangeVale;
                if(r>=70)
                    r+=ChangeVale;
                else
                    r-=ChangeVale;
                if(b>=255)                                  //�жϼ�����RGBֵ�Ƿ񳬹�255��С��0�������Ļ�����ֵ��Ϊ0��255
                    *(pImgData+i*lineByte+j)=255;
                else
                    if(b<=0)
                        *(pImgData+i*lineByte+j)=0;
                    else
                        *(pImgData+i*lineByte+j)=b;
                if(g>=255)
                    *(pImgData+i*lineByte+j+1)=255;
                else
                    if(g<=0)
                        *(pImgData+i*lineByte+j+1)=0;
                    else
                        *(pImgData+i*lineByte+j+1)=g;
                if(r>=255)
                    *(pImgData+i*lineByte+j+2)=255;
                else
                    if(r<=0)
                        *(pImgData+i*lineByte+j+2)=0;
                    else
                        *(pImgData+i*lineByte+j+2)=r;
            }
        }
}

int main ()
{
    char fileName[30];                        //�����ͼ������
    char fname[]="f://dmt//fuben.txt";
    char *buf;                                //�����ļ���ȡ������
    char *p;

    HWND wnd;                                 //���ھ��
    HDC dc;                                   //��ͼ�豸�������
    FILE *fp,*file;                           //�����ļ�ָ��
    FILE *fpw;                                //���屣���ļ�ָ��
    DWORD w,h;                                //�����ȡͼ��ĳ��Ϳ�
    DWORD bitCorlorUsed;                      //����
    DWORD bitSize;                            //����ͼ��Ĵ�С
    BITMAPFILEHEADER bf;                      //ͼ���ļ�ͷ
    BITMAPINFOHEADER bi;                      //ͼ���ļ�ͷ��Ϣ
    cout<<"������Ҫ���ļ������֣�";
    cin>>fileName;
    if((fp=fopen(fileName,"rb"))==NULL)
    {
       cout<<"�ļ�δ�ҵ���";
       exit(0);
    }
    fread(&bf,sizeof(BITMAPFILEHEADER),1,fp);//��ȡBMP�ļ�ͷ�ļ�
    fread(&bi,sizeof(BITMAPINFOHEADER),1,fp);//��ȡBMP�ļ�ͷ�ļ���Ϣ
    w=bi.biWidth;                            //��ȡͼ��Ŀ�
    h=bi.biHeight;                           //��ȡͼ��ĸ�
    bitSize=bi.biSizeImage;                  //��ȡͼ���size
    buf=(char*)malloc(w*h*3);                //���仺������С
    fseek(fp,long(sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)),0);//��λ��������ʼλ��
    fread(buf,1,w*h*3,fp);                   //��ʼ��ȡ����
    wnd=GetForegroundWindow();               //��ȡ���ھ��
    dc=GetDC(wnd);                           //��ȡ��ͼ�豸

    write_txt(w,h,buf);
    if((file = fopen(fname,"rb"))==NULL)
        return 0;
    fread(buf,1,w*h*3-h*(((3 * (w + 1) / 4) * 4)-3*w),file);
    cout<<"�������������Ĳ�����"<<endl<<"1��תΪ�Ҷ�ͼ"<<endl<<"2��ת��Ϊ��ֵͼ"<<endl<<"3���ı�Աȶ�"<<endl;

    getchar();
    switch(getchar())
    {
        case '1': rgbTohd(w*h,buf); save(w,h,buf); break;
        case '2': rgbTowb(w*h,buf); save(w,h,buf); break;
        case '3':
            getchar(); cout<<"������ֵ��"<<endl; int x; scanf("%d",&x); cout<<x;
            contrastRatiochange(buf,h,w,((3 * (w + 1) / 4) * 4),x); save(w,h,buf); break;
    }

    return 0;
}
