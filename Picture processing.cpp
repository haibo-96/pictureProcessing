#include<iostream>
#include<windows.h>
#include<malloc.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

using namespace std;

int pdqb(unsigned short x)                              //判断
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
    x=((3 * (width + 1) / 4) * 4)-3*width;                  //每行应该补的字节数
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
            ipos = 3 * (width * i + j) + k;                 //同write_bmp
            line[3*j] = rgb[ipos];
            line[3*j+1] = rgb[ipos + 1];
            line[3*j+2] = rgb[ipos + 2];
        }
        fwrite(line, bytesPerLine, 1, file);
        k = k + x;                                          //计算应该跳过的字节数
    }

    free(line);
    fclose(file);

    return(1);
}

int write_bmp(const char *filename, int width, int height, char *rgb)   //写入bmp文件
{
    int i, j, ipos;
    int bytesPerLine;
    unsigned char *line;

    FILE *file;
    BITMAPFILEHEADER bf;                      //图像文件头
    BITMAPINFOHEADER bi;                      //图像文件头信息

    /* The length of each line must be a multiple of 4 bytes */

    bytesPerLine = (3 * (width + 1) / 4) * 4; //计算每行的字节数
    bf.bfType=0x4d42;                        //设置文件头信息
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

    fwrite(&bf.bfType, 2, 1, file);                         //写入文件头信息
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
            ipos = 3 * (width * i + j);                     //找到当前像素点的位置
            line[3*j] = rgb[ipos];                          //将当前点的RGB数据写入line中
            line[3*j+1] = rgb[ipos + 1];
            line[3*j+2] = rgb[ipos + 2];
        }
        fwrite(line, bytesPerLine, 1, file);                //将一行RGB值写入文件中
    }

    free(line);
    fclose(file);

    return(1);
}

void save(int w,int h,char* buf)                            //保存文件
{
    char fname[30];
    cout<<"请输入要保存文件的名字：";
    cin>>fname;
    write_bmp(fname,w,h,buf);
}

void rgbTohd(int i,char *rgb)                               //灰度转换
{
     unsigned short r,g,b,gary;

     for(int j=0;j<=i;j++)
     {
         b=pdqb((unsigned short)(*rgb));                    //读取RGB值
         g=pdqb((unsigned short)(*(rgb+1)));
         r=pdqb((unsigned short)(*(rgb+2)));
         gary=(299*r+587*g+114*b+500)/1000;                 //公式计算Gary
         (*rgb)=(*(rgb+1))=(*(rgb+2))=gary;                 //重新赋值给RGB
         rgb=rgb+3;
     }

}

void rgbTowb(int i,char* rgb)                               //二值图转换
{
     rgbTohd(i,rgb);                                        //先将图片转为灰度

     for(int j=0;j<=i;j++)
     {
         if((*rgb)<=50)                                     //设置常量为50
            (*rgb)=(*(rgb+1))=(*(rgb+2))=0;                 //小干50赋值为0
         else
            (*rgb)=(*(rgb+1))=(*(rgb+2))=255;               //大于50赋值为255
         rgb=rgb+3;
     }
}

void contrastRatiochange(char* pImgData,int height,int width,int lineByte,int ChangeVale)   //对比度调节
{
    for(int i=0;i<height;i++)
        {
            for(int j=0;j<3*width;j=j+3)                    //读取RGB值
            {   int b=pdqb(*(pImgData+i*lineByte+j));
                int g=pdqb(*(pImgData+i*lineByte+j+1));
                int r=pdqb(*(pImgData+i*lineByte+j+2));
                if(b>=70)                                   //依次判断RGB与常数的关系，大于则加用户给定的数，小于则减去用户给定的数
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
                if(b>=255)                                  //判断计算后的RGB值是否超过255或小于0，超出的话将其值设为0或255
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
    char fileName[30];                        //定义打开图像名字
    char fname[]="f://dmt//fuben.txt";
    char *buf;                                //定义文件读取缓冲区
    char *p;

    HWND wnd;                                 //窗口句柄
    HDC dc;                                   //绘图设备环境句柄
    FILE *fp,*file;                           //定义文件指针
    FILE *fpw;                                //定义保存文件指针
    DWORD w,h;                                //定义读取图像的长和宽
    DWORD bitCorlorUsed;                      //定义
    DWORD bitSize;                            //定义图像的大小
    BITMAPFILEHEADER bf;                      //图像文件头
    BITMAPINFOHEADER bi;                      //图像文件头信息
    cout<<"请输入要打开文件的名字：";
    cin>>fileName;
    if((fp=fopen(fileName,"rb"))==NULL)
    {
       cout<<"文件未找到！";
       exit(0);
    }
    fread(&bf,sizeof(BITMAPFILEHEADER),1,fp);//读取BMP文件头文件
    fread(&bi,sizeof(BITMAPINFOHEADER),1,fp);//读取BMP文件头文件信息
    w=bi.biWidth;                            //获取图像的宽
    h=bi.biHeight;                           //获取图像的高
    bitSize=bi.biSizeImage;                  //获取图像的size
    buf=(char*)malloc(w*h*3);                //分配缓冲区大小
    fseek(fp,long(sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)),0);//定位到像素起始位置
    fread(buf,1,w*h*3,fp);                   //开始读取数据
    wnd=GetForegroundWindow();               //获取窗口句柄
    dc=GetDC(wnd);                           //获取绘图设备

    write_txt(w,h,buf);
    if((file = fopen(fname,"rb"))==NULL)
        return 0;
    fread(buf,1,w*h*3-h*(((3 * (w + 1) / 4) * 4)-3*w),file);
    cout<<"请输入您想做的操作："<<endl<<"1、转为灰度图"<<endl<<"2、转换为二值图"<<endl<<"3、改变对比度"<<endl;

    getchar();
    switch(getchar())
    {
        case '1': rgbTohd(w*h,buf); save(w,h,buf); break;
        case '2': rgbTowb(w*h,buf); save(w,h,buf); break;
        case '3':
            getchar(); cout<<"请输入值："<<endl; int x; scanf("%d",&x); cout<<x;
            contrastRatiochange(buf,h,w,((3 * (w + 1) / 4) * 4),x); save(w,h,buf); break;
    }

    return 0;
}
