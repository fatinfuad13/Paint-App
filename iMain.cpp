# include "iGraphics.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#define screenWidth 1500
#define screenHeight 800
#define canvasHeight 700
int pixelColor[3];
int total_points=0,total_circles=0,total_rect=0,total_lines=0,total_text=0,total_ellipses=0,total_dynamic_circles=0;
int curr_red,curr_blue,curr_green;
int len=0;
int curr_points_size=7,curr_circles_size=50,curr_rect_length=100,curr_rect_width=50,curr_major=50,curr_minor=25;
int curr_line_width=25;//arbitrary
int curr_font=1;//1-7 for fonts

int curr_filled=1;

int tool_width=2;//

int mode=1;
//Mode 1 for homescreen, Mode 2 for drawing board

int tool=1;// tool=1 for free draw and eraser,tool=2 for circle,tool=3 for rectangle,tool=4,5 for start end points of a line,tool=6 for text,tool=7 for ellipse
//tools 8,9 for dynamic circle

int save_cnt=0;

char save_filename[100], save_names[100][100];
int last_point_index=-1;//

int clear_screen=0;

typedef struct{


  int x,y,r,g,b,size;//In a structure type of "points" we keep all information about an individual point or shape



}points/*,circles*/;

typedef struct
{

    int x,y,r,g,b,filled,size;

}circles;

typedef struct
{

    int x,y,dx,dy,r,g,b,filled,size;

}rectangles;

typedef struct
{

    double x1,y1,x2,y2,r,g,b,width;//int


}lines;


typedef struct
{
int x,y,r,g,b,font;
char text[100];

} textbox;

typedef struct
{

    int x,y,da,db,r,g,b,filled;


}ellipses;


typedef struct//
{

    double x1,x2,y1,y2,r,g,b,filled;//


}dynamic_circles;//





points *PointsArray= (points *) malloc(9999999 * sizeof(points));
//Dynamically allocating memory for 9999999 such points,we can access the points using the pointer PointsArray


circles *CirclesArray= (circles *) malloc(100 * sizeof(circles));
int * circlesAt=(int *)malloc(100*sizeof(int));

rectangles *RectanglesArray= (rectangles *) malloc(100 * sizeof(rectangles));
int * rectanglesAt=(int *)malloc(100*sizeof(int));


lines *LinesArray=(lines *)malloc(100 * sizeof(lines));
int *linesAt=(int *)malloc(100*sizeof(int));


textbox *TextArray=(textbox*)malloc(10*sizeof(textbox));
int *textAt=(int *)malloc(10*sizeof(int));

ellipses *EllipsesArray=(ellipses*)malloc(100*sizeof(ellipses));
int *ellipsesAt=(int *) malloc(sizeof(int)*100);



dynamic_circles *DynamicCirclesArray= (dynamic_circles*) malloc(100*sizeof(dynamic_circles));//
int *dynamic_circlesAt=(int *)malloc(sizeof(int)*100);//






void SaveFile(char save_filename[])
{


  FILE * to_write = fopen (save_filename, "wb");

   if (to_write==NULL) printf("Failed to Save\n");

   if (fwrite(&total_points, sizeof(int), 1, to_write)!=1)  printf("Failed to Save\n");
   if (fwrite(&total_text, sizeof(int), 1, to_write)!=1)  printf("Failed to Save\n");
   if (fwrite(&total_circles, sizeof(int), 1, to_write)!=1)  printf("Failed to Save\n");
   if (fwrite(&total_dynamic_circles, sizeof(int), 1, to_write)!=1)  printf("Failed to Save\n");
   if (fwrite(&total_ellipses, sizeof(int), 1, to_write)!=1)  printf("Failed to Save\n");
   if (fwrite(&total_rect, sizeof(int), 1, to_write)!=1)  printf("Failed to Save\n");
   if (fwrite(&total_lines, sizeof(int), 1, to_write)!=1)  printf("Failed to Save\n");


   if (fwrite(textAt, sizeof(int), total_text, to_write)!=total_text) printf("Failed to Save\n");
   if (fwrite(circlesAt, sizeof(int), total_circles, to_write)!=total_circles) printf("Failed to Save\n");
   if (fwrite(rectanglesAt, sizeof(int), total_rect, to_write)!=total_rect) printf("Failed to Save\n");
   if (fwrite(ellipsesAt, sizeof(int), total_ellipses, to_write)!=total_ellipses) printf("Failed to Save\n");
   if (fwrite(linesAt, sizeof(int), total_lines, to_write)!=total_lines) printf("Failed to Save\n");
   if (fwrite(dynamic_circlesAt, sizeof(int), total_dynamic_circles, to_write)!=total_dynamic_circles) printf("Failed to Save\n");


   if (fwrite(PointsArray, sizeof(points), total_points, to_write)!=total_points) printf("Failed to Save\n");
   if (fwrite(CirclesArray, sizeof(circles), total_circles, to_write)!=total_circles) printf("Failed to Save\n");
   if (fwrite(RectanglesArray, sizeof(rectangles), total_rect, to_write)!=total_rect) printf("Failed to Save\n");
    if (fwrite(EllipsesArray, sizeof(ellipses), total_ellipses, to_write)!=total_ellipses) printf("Failed to Save\n");
    if (fwrite(DynamicCirclesArray, sizeof(dynamic_circles), total_dynamic_circles, to_write)!=total_dynamic_circles) printf("Failed to Save\n");
    if (fwrite(LinesArray, sizeof(lines), total_lines, to_write)!=total_lines) printf("Failed to Save\n");
   if (fwrite(TextArray, sizeof(textbox), total_text, to_write)!=total_text) printf("Failed to Save\n");



   if(fclose(to_write)==EOF) printf("Failed to Save\n");
   FILE * save_list = fopen("saves", "a+");

   if (save_list==NULL) printf("Failed to Save\n");

   char sfn_var [100];                 // a var str to store save_filename

    strcpy(sfn_var, save_filename);
    strcat(sfn_var, "\n");


   bool exists=false;

    while (!feof(save_list)) {

        char read[100]; fgets(read, 100, save_list);

        if(!strcmp(read, sfn_var)){                        // if matching case found
            exists=true;
            break;
            }
    }                                                            // returns 0 if a matching case found

    if (!exists) {
        fputs(save_filename, save_list);
        fputc('\n', save_list);
    }

    fclose(save_list);









}



void LoadFile(char save_name[])
{



    FILE * from_save = fopen(save_name, "rb");

    if (from_save==NULL)
     mode=2;



     if (fread(&total_points, sizeof(int), 1, from_save)!=1)  mode=2;
   if (fread(&total_text, sizeof(int), 1, from_save)!=1)  mode=2;
   if (fread(&total_circles, sizeof(int), 1, from_save)!=1)  mode=2;
   if (fread(&total_dynamic_circles, sizeof(int), 1, from_save)!=1)  mode=2;
   if (fread(&total_ellipses, sizeof(int), 1, from_save)!=1)  mode=2;
   if (fread(&total_rect, sizeof(int), 1, from_save)!=1)  mode=2;
   if (fread(&total_lines, sizeof(int), 1, from_save)!=1)  mode=2;



   if (fread(textAt, sizeof(int), total_text, from_save)!=total_text) mode=2;
   if (fread(circlesAt, sizeof(int), total_circles,from_save )!=total_circles) mode=2;
   if (fread(rectanglesAt, sizeof(int), total_rect, from_save)!=total_rect) mode=2;
   if (fread(ellipsesAt, sizeof(int), total_ellipses, from_save)!=total_ellipses) mode=2;
   if (fread(linesAt, sizeof(int), total_lines, from_save)!=total_lines) mode=2;
   if (fread(dynamic_circlesAt, sizeof(int), total_dynamic_circles, from_save)!=total_dynamic_circles) mode=2;





   if (fread(PointsArray, sizeof(points), total_points, from_save)!=total_points) mode=2;
   if (fread(CirclesArray, sizeof(circles), total_circles, from_save)!=total_circles) mode=2;
   if (fread(RectanglesArray, sizeof(rectangles), total_rect,from_save)!=total_rect) mode=2;
    if (fread(EllipsesArray, sizeof(ellipses), total_ellipses, from_save)!=total_ellipses) mode=2;
    if (fread(DynamicCirclesArray, sizeof(dynamic_circles), total_dynamic_circles, from_save)!=total_dynamic_circles) mode=2;
    if (fread(LinesArray, sizeof(lines), total_lines, from_save)!=total_lines) mode=2;
   if (fread(TextArray, sizeof(textbox), total_text, from_save)!=total_text) mode=2;













   if(fclose(from_save)==EOF) mode=2;


}













void readList()
{

        FILE * save_list= fopen("saves", "r");
        if (save_list==NULL) {mode=2; return;}

            while (!feof(save_list)) {                        // if no prob

                char c=fgetc(save_list);
                if (c=='\n') save_cnt++;

            }

        rewind(save_list);

        for (int i=0; i<save_cnt; i++) {

            fgets(save_names[i], 100, save_list);

            save_names[i][strlen(save_names[i])-1]='\0';      // Adding null char to  end of each filename read

          }

          fclose(save_list);


}




void init();
void freeMem();
void erase();














void show_things()
{

  //SHOWING SELECTED TOOL+COLOUR
   if(tool==1&&curr_red==0&&curr_blue==0&&curr_green==0&&mode==2)
   {
       iSetColor(255,255,255);
       iText(1400,screenHeight-75,"Eraser",GLUT_BITMAP_9_BY_15);




   }



else if(tool==1&&mode==2)
   {


       iSetColor(curr_red,curr_green,curr_blue);
       iText(1400,screenHeight-75,"Brush",GLUT_BITMAP_9_BY_15);

       iSetColor(255,255,255);
       if(curr_points_size==5) iText(1350,screenHeight-90,"Tool Width:1",GLUT_BITMAP_9_BY_15);
    else if(curr_points_size==7) iText(1350,screenHeight-90,"Tool Width:2",GLUT_BITMAP_9_BY_15);
    else if(curr_points_size==9) iText(1350,screenHeight-90,"Tool Width:3",GLUT_BITMAP_9_BY_15);
    else if(curr_points_size==11) iText(1350,screenHeight-90,"Tool Width:4",GLUT_BITMAP_9_BY_15);
    else if(curr_points_size==13) iText(1350,screenHeight-90,"Tool Width:5",GLUT_BITMAP_9_BY_15);








   }


else if(tool==2)
{
    iSetColor(curr_red,curr_green,curr_blue);
    if(curr_filled==1) iText(1350,screenHeight-75,"Filled Circle",GLUT_BITMAP_9_BY_15);
    else iText(1350,screenHeight-75,"Border Circle",GLUT_BITMAP_9_BY_15);

    iSetColor(255,255,255);
    if(curr_circles_size==50) iText(1350,screenHeight-90,"Tool Width:1",GLUT_BITMAP_9_BY_15);
    else if(curr_circles_size==70) iText(1350,screenHeight-90,"Tool Width:2",GLUT_BITMAP_9_BY_15);
    else if(curr_circles_size==90) iText(1350,screenHeight-90,"Tool Width:3",GLUT_BITMAP_9_BY_15);
    else if(curr_circles_size==110) iText(1350,screenHeight-90,"Tool Width:4",GLUT_BITMAP_9_BY_15);
    else if(curr_circles_size==130) iText(1350,screenHeight-90,"Tool Width:5",GLUT_BITMAP_9_BY_15);









}


else if(tool==3)
{

  iSetColor(curr_red,curr_green,curr_blue);
    if(curr_filled==1) iText(1350,screenHeight-75,"Filled Rectangle",GLUT_BITMAP_9_BY_15);
    else iText(1350,screenHeight-75,"Border Rectangle",GLUT_BITMAP_9_BY_15);


    iSetColor(255,255,255);
       if(curr_rect_length==100) iText(1350,screenHeight-90,"Tool Width:1",GLUT_BITMAP_9_BY_15);
    else if(curr_rect_length==150) iText(1350,screenHeight-90,"Tool Width:2",GLUT_BITMAP_9_BY_15);
    else if(curr_rect_length==200) iText(1350,screenHeight-90,"Tool Width:3",GLUT_BITMAP_9_BY_15);
    else if(curr_rect_length==250) iText(1350,screenHeight-90,"Tool Width:4",GLUT_BITMAP_9_BY_15);
    else if(curr_rect_length==300) iText(1350,screenHeight-90,"Tool Width:5",GLUT_BITMAP_9_BY_15);







}

else if(tool==4||tool==5)
{
  iSetColor(curr_red,curr_green,curr_blue);
  iText(1400,screenHeight-75,"Line",GLUT_BITMAP_9_BY_15);

  iSetColor(255,255,255);
       if(curr_line_width==5) iText(1350,screenHeight-90,"Tool Width:1",GLUT_BITMAP_9_BY_15);
    else if(curr_line_width==15) iText(1350,screenHeight-90,"Tool Width:2",GLUT_BITMAP_9_BY_15);
    else if(curr_line_width==25) iText(1350,screenHeight-90,"Tool Width:3",GLUT_BITMAP_9_BY_15);
    else if(curr_line_width==35) iText(1350,screenHeight-90,"Tool Width:4",GLUT_BITMAP_9_BY_15);
    else if(curr_line_width==45) iText(1350,screenHeight-90,"Tool Width:5",GLUT_BITMAP_9_BY_15);





}

else if(tool==6)
{

   iSetColor(curr_red,curr_green,curr_blue);
   iText(1400,screenHeight-75,"Text",GLUT_BITMAP_9_BY_15);

   iSetColor(255,255,255);
    if(curr_font==1) iText(1400,screenHeight-90,"Font 1",GLUT_BITMAP_8_BY_13);
    else if(curr_font==2) iText(1400,screenHeight-90,"Font 2",GLUT_BITMAP_9_BY_15);
    else if(curr_font==3) iText(1400,screenHeight-90,"Font 3",GLUT_BITMAP_HELVETICA_10);
    else if(curr_font==4) iText(1400,screenHeight-90,"Font 4",GLUT_BITMAP_HELVETICA_12);
    else if(curr_font==5) iText(1400,screenHeight-90,"Font 5",GLUT_BITMAP_HELVETICA_18);
    else if(curr_font==6) iText(1400,screenHeight-90,"Font 6",GLUT_BITMAP_TIMES_ROMAN_10);
    else if(curr_font==7) iText(1400,screenHeight-95,"Font 7",GLUT_BITMAP_TIMES_ROMAN_24);





}

else if(tool==7)
{

iSetColor(curr_red,curr_green,curr_blue);
    if(curr_filled==1) iText(1350,screenHeight-75,"Filled Ellipse",GLUT_BITMAP_9_BY_15);
    else iText(1350,screenHeight-75,"Border Ellipse",GLUT_BITMAP_9_BY_15);



    iSetColor(255,255,255);
       if(curr_major==50) iText(1350,screenHeight-90,"Tool Width:1",GLUT_BITMAP_9_BY_15);
    else if(curr_major==100) iText(1350,screenHeight-90,"Tool Width:2",GLUT_BITMAP_9_BY_15);
    else if(curr_major==150) iText(1350,screenHeight-90,"Tool Width:3",GLUT_BITMAP_9_BY_15);
    else if(curr_major==180) iText(1350,screenHeight-90,"Tool Width:4",GLUT_BITMAP_9_BY_15);
    else if(curr_major==200) iText(1350,screenHeight-90,"Tool Width:5",GLUT_BITMAP_9_BY_15);









}

else if(tool==8||tool==9)
{

    iSetColor(curr_red,curr_green,curr_blue);
    if(curr_filled==1) iText(1335,screenHeight-75,"Dynamic Filled Circle",GLUT_BITMAP_8_BY_13);
     else iText(1335,screenHeight-75,"Dynamic Border Circle",GLUT_BITMAP_8_BY_13);




}




/*if(tool!=6)
{

    iSetColor(255,255,255);

    if(tool_width==1) iText(1350,screenHeight-90,"Tool Width:1",GLUT_BITMAP_9_BY_15);
    else if(tool_width==2) iText(1350,screenHeight-90,"Tool Width:2",GLUT_BITMAP_9_BY_15);
    else if(tool_width==3) iText(1350,screenHeight-90,"Tool Width:3",GLUT_BITMAP_9_BY_15);
    else if(tool_width==4) iText(1350,screenHeight-90,"Tool Width:4",GLUT_BITMAP_9_BY_15);
    else if(tool_width==5) iText(1350,screenHeight-90,"Tool Width:5",GLUT_BITMAP_9_BY_15);



}*/






//SHOWING SELECTED TOOL+COLOUR














    int i,c,r,l,txt,e,dc;
    for( i=0,c=0,r=0,l=0,txt=0,e=0,dc=0;i<total_points;i++)
    {


   if(i==circlesAt[c])
   {


   //if(CirclesArray[c].y<=canvasHeight)




       iSetColor(CirclesArray[c].r,CirclesArray[c].g,CirclesArray[c].b);
        if(CirclesArray[c].y+CirclesArray[c].size<=canvasHeight)

        {
             if(CirclesArray[c].filled==1) iFilledCircle(CirclesArray[c].x,CirclesArray[c].y,CirclesArray[c].size);
            else iCircle(CirclesArray[c].x,CirclesArray[c].y,CirclesArray[c].size);



        }
        c++;
         //printf("%d\n",CirclesArray[i].y);





   }


   else if(i==rectanglesAt[r])
   {


   //if(CirclesArray[c].y<=canvasHeight)




       iSetColor(RectanglesArray[r].r,RectanglesArray[r].g,RectanglesArray[r].b);
        if(RectanglesArray[r].y+RectanglesArray[r].dy<=canvasHeight)
        {


           if(RectanglesArray[r].filled==1) iFilledRectangle(RectanglesArray[r].x,RectanglesArray[r].y,RectanglesArray[r].dx,RectanglesArray[r].dy);
           else iRectangle(RectanglesArray[r].x,RectanglesArray[r].y,RectanglesArray[r].dx,RectanglesArray[r].dy);


        }

        r++;
         //printf("%d\n",CirclesArray[i].y);





   }



   else if(i==linesAt[l])
   {


       iSetColor(LinesArray[l].r,LinesArray[l].g,LinesArray[l].b);
       if(LinesArray[l].y1<=canvasHeight&&LinesArray[l].y2<=canvasHeight)
       {

           iLine(LinesArray[l].x1,LinesArray[l].y1,LinesArray[l].x2,LinesArray[l].y2);

           /*for(int i=0;i<50;i++)//
           {
               iLine(LinesArray[l].x1+0.1*i,LinesArray[l].y1+0.1*i,LinesArray[l].x2+0.1*i,LinesArray[l].y2+0.1*i);


           }

           for(int i=0;i<50;i++)//
           {
               iLine(LinesArray[l].x1-0.1*i,LinesArray[l].y1-0.1*i,LinesArray[l].x2-0.1*i,LinesArray[l].y2-0.1*i);


           }*/

for (int i = 0; i < LinesArray[l].width; i++)
{
    iLine(LinesArray[l].x1 + 0.1 * i, LinesArray[l].y1, LinesArray[l].x2 + 0.1 * i, LinesArray[l].y2);
}


  for (int i = 0; i <LinesArray[l].width; i++)

    {
    iLine(LinesArray[l].x1 - 0.1 * i, LinesArray[l].y1, LinesArray[l].x2 - 0.1 * i, LinesArray[l].y2);
    }


  for (int i = 0; i < LinesArray[l].width; i++)
{
    iLine(LinesArray[l].x1 , LinesArray[l].y1+ 0.1 * i, LinesArray[l].x2, LinesArray[l].y2+ 0.1 * i);
}


    for (int i = 0; i <LinesArray[l].width; i++)
{
    iLine(LinesArray[l].x1 , LinesArray[l].y1- 0.1 * i, LinesArray[l].x2, LinesArray[l].y2-0.1 * i);
}









       }
       l++;





   }


//
else if(i==dynamic_circlesAt[dc])
{

    iSetColor(DynamicCirclesArray[dc].r,DynamicCirclesArray[dc].g,DynamicCirclesArray[dc].b);

   double X=(DynamicCirclesArray[dc].x1+DynamicCirclesArray[dc].x2)/2;
   double Y=(DynamicCirclesArray[dc].y1+DynamicCirclesArray[dc].y2)/2;
    double R=sqrt((DynamicCirclesArray[dc].x1-X)*(DynamicCirclesArray[dc].x1-X)+(DynamicCirclesArray[dc].y1-Y)*(DynamicCirclesArray[dc].y1-Y));

    if((Y+R)<=canvasHeight)
    {



        if(DynamicCirclesArray[dc].filled==1) iFilledCircle(X,Y,R);
        else iCircle(X,Y,R);





    }


    dc++;


}









  else if(i==textAt[txt])
  {

     if(TextArray[txt].font==1)
     {



      iSetColor(TextArray[txt].r,TextArray[txt].g,TextArray[txt].b);
      if(TextArray[txt].y<=canvasHeight) iText(TextArray[txt].x,TextArray[txt].y,TextArray[txt].text,GLUT_BITMAP_8_BY_13);

      txt++;


     }


     else if(TextArray[txt].font==2)
     {



      iSetColor(TextArray[txt].r,TextArray[txt].g,TextArray[txt].b);
      if(TextArray[txt].y<=canvasHeight) iText(TextArray[txt].x,TextArray[txt].y,TextArray[txt].text,GLUT_BITMAP_9_BY_15);

      txt++;


     }

     else if(TextArray[txt].font==3)
     {



      iSetColor(TextArray[txt].r,TextArray[txt].g,TextArray[txt].b);
      if(TextArray[txt].y<=canvasHeight) iText(TextArray[txt].x,TextArray[txt].y,TextArray[txt].text,GLUT_BITMAP_HELVETICA_10);

      txt++;


     }

     else if(TextArray[txt].font==4)
     {



      iSetColor(TextArray[txt].r,TextArray[txt].g,TextArray[txt].b);
      if(TextArray[txt].y<=canvasHeight) iText(TextArray[txt].x,TextArray[txt].y,TextArray[txt].text,GLUT_BITMAP_HELVETICA_12);

      txt++;


     }


     else if(TextArray[txt].font==5)
     {



      iSetColor(TextArray[txt].r,TextArray[txt].g,TextArray[txt].b);
      if(TextArray[txt].y<=canvasHeight) iText(TextArray[txt].x,TextArray[txt].y,TextArray[txt].text,GLUT_BITMAP_HELVETICA_18);

      txt++;


     }

     else if(TextArray[txt].font==6)
     {



      iSetColor(TextArray[txt].r,TextArray[txt].g,TextArray[txt].b);
      if(TextArray[txt].y<=canvasHeight) iText(TextArray[txt].x,TextArray[txt].y,TextArray[txt].text,GLUT_BITMAP_TIMES_ROMAN_10);

      txt++;


     }

     else if(TextArray[txt].font==7)
     {



      iSetColor(TextArray[txt].r,TextArray[txt].g,TextArray[txt].b);
      if(TextArray[txt].y<=canvasHeight) iText(TextArray[txt].x,TextArray[txt].y,TextArray[txt].text,GLUT_BITMAP_TIMES_ROMAN_24);

      txt++;


     }






  }


  else if(i==ellipsesAt[e])
  {

      iSetColor(EllipsesArray[e].r,EllipsesArray[e].g,EllipsesArray[e].b);

         if(EllipsesArray[e].y<=canvasHeight-EllipsesArray[e].db)
         {


           if(EllipsesArray[e].filled==1) iFilledEllipse(EllipsesArray[e].x,EllipsesArray[e].y,EllipsesArray[e].da,EllipsesArray[e].db,100);
           else iEllipse(EllipsesArray[e].x,EllipsesArray[e].y,EllipsesArray[e].da,EllipsesArray[e].db,100);


         }

      e++;


  }






    else
    {
        if(PointsArray[i].y+PointsArray[i].size<=canvasHeight) //for drawing in only canvas
   {


        iSetColor(PointsArray[i].r,PointsArray[i].g,PointsArray[i].b);
        iFilledCircle(PointsArray[i].x,PointsArray[i].y,PointsArray[i].size);
        //Points are basically very small circles
        //For all the points we display them as circles of chosen width and color



   }



    }



   }









  /*if(tool==1&&curr_red==0&&curr_blue==0&&curr_green==0)
   {
       iSetColor(255,255,255);
       iText(1400,screenHeight-75,"Eraser",GLUT_BITMAP_9_BY_15);




   }



else if(tool==1)
   {

       iText(1400,screenHeight-75,"Brush",GLUT_BITMAP_9_BY_15);


   }*/



















}













double x,y, r = 20;


int a=0;

//int color_x=0,color_y=screenHeight-25;


void iDraw()
{

  iClear();
  if(mode==1)
{

    iShowBMP(39,0,"Image//home_screen.bmp");
    iSetColor(100,0,200);
    //iFilledRectangle(500+150,300+63,200,50);
    iFilledEllipse(750,388,100,25);

    iFilledEllipse(750,288,100,25);


    iFilledEllipse(750,288,100,25);

    iFilledEllipse(750,188,100,25);


    iSetColor(255,255,255);
    iText(540+150,320+63,"Start Drawing",GLUT_BITMAP_9_BY_15);
    iText(540+150-10,220+63,"  Instructions",GLUT_BITMAP_9_BY_15);
    iText(540+150,120+63,"View Drawing",GLUT_BITMAP_9_BY_15);









}


  else if(mode==2)
  {








   iSetColor(0,0,0);
   iFilledRectangle(0,0,screenWidth,screenHeight); //Setting background color to black
   show_things();

   iSetColor(255,255,255);
   iLine(0,canvasHeight,screenWidth,canvasHeight);




int color_x=0,color_y=screenHeight-25;

   for(int i=0;i<6;i++)
   {

   iSetColor(255-35*i,0,0);
   iFilledRectangle(color_x,color_y,25,25);
   color_x+=25;


   }


   for(int i=0;i<6;i++)
   {

   iSetColor(0,255-35*i,0);
   iFilledRectangle(color_x,color_y,25,25);
   color_x+=25;


   }

   for(int i=0;i<6;i++)
   {

   iSetColor(0,0,255-35*i);
   iFilledRectangle(color_x,color_y,25,25);
   color_x+=25;


   }


   for(int i=0;i<6;i++)
   {

   iSetColor(0,255-35*i,255-35*i);
   iFilledRectangle(color_x,color_y,25,25);
   color_x+=25;


   }

   for(int i=0;i<6;i++)
   {

   iSetColor(255-35*i,255-35*i,0);
   iFilledRectangle(color_x,color_y,25,25);
   color_x+=25;


   }

   for(int i=0;i<6;i++)
   {

   iSetColor(255-35*i,0,255-35*i);
   iFilledRectangle(color_x,color_y,25,25);
   color_x+=25;


   }







    for(int i=0;i<6;i++)
   {

   iSetColor(255-35*i,255-35*i,255-35*i);
   iFilledRectangle(color_x,color_y,25,25);
   color_x+=25;


   }



   for(int i=0;i<6;i++)
   {

   iSetColor(150-i*20,70+i*20,i*20);
   iFilledRectangle(color_x,color_y,25,25);
   color_x+=25;


   }

   //color_x+=25;
   iSetColor(255,255,255);
   iFilledRectangle(color_x,color_y,25,25);
   iShowBMP(color_x,color_y,"Image//brush.bmp");


   iText(color_x,color_y-25,"Brush",GLUT_BITMAP_9_BY_15);

   color_x+=50;



   iFilledRectangle(color_x,color_y,25,25);
   iShowBMP(color_x,color_y,"Image//eraser.bmp");


   iText(color_x-10,color_y-25," Eraser",GLUT_BITMAP_9_BY_15);

   color_x+=75;
   //printf("%d",color_x);

iText(color_x+30,color_y-25,"Tool Width",GLUT_BITMAP_9_BY_15);


   for(int i=0;i<5;i++)
   {



   iFilledCircle(color_x,color_y+10,5+2*i);

   color_x+=35;




   }


iSetColor(255,255,255);
iFilledRectangle(0,screenHeight-75,25,25);
iShowBMP(0,screenHeight-75,"Image//circle.bmp");


iFilledRectangle(75,screenHeight-75,25,25);
iShowBMP(75,screenHeight-75,"Image//rectangle.bmp");

iFilledRectangle(150,screenHeight-75,25,25);
iShowBMP(150,screenHeight-75,"Image//line.bmp");

iFilledRectangle(225,screenHeight-75,25,25);
iShowBMP(225,screenHeight-75,"Image//text.bmp");

iFilledRectangle(300,screenHeight-75,25,25);
iShowBMP(300,screenHeight-75,"Image//ellipse.bmp");

iFilledRectangle(375,screenHeight-75,25,25);


iFilledRectangle(450,screenHeight-75,25,25);



iText(0,screenHeight-90,"Circle ",GLUT_BITMAP_9_BY_15);
iText(52,screenHeight-90," Rectangle",GLUT_BITMAP_9_BY_15);
iText(145,screenHeight-90," Line",GLUT_BITMAP_9_BY_15);
iText(220,screenHeight-90,"Text",GLUT_BITMAP_9_BY_15);
iText(295,screenHeight-90,"Ellipse",GLUT_BITMAP_9_BY_15);
iText(370,screenHeight-90,"Filled",GLUT_BITMAP_9_BY_15);
iText(445,screenHeight-90,"Border",GLUT_BITMAP_9_BY_15);


iFilledRectangle(525,screenHeight-75,25,25);
iFilledRectangle(600,screenHeight-75,25,25);
iFilledRectangle(675,screenHeight-75,25,25);
iFilledRectangle(750,screenHeight-75,25,25);
iFilledRectangle(825,screenHeight-75,25,25);
iFilledRectangle(900,screenHeight-75,25,25);
iFilledRectangle(975,screenHeight-75,25,25);

iText(520,screenHeight-90,"Font 1",GLUT_BITMAP_8_BY_13);
iText(595,screenHeight-90,"Font 2",GLUT_BITMAP_9_BY_15);
iText(670,screenHeight-90," Font 3",GLUT_BITMAP_HELVETICA_10);
iText(745,screenHeight-90,"Font 4",GLUT_BITMAP_HELVETICA_12);
iText(820,screenHeight-90,"Font 5",GLUT_BITMAP_HELVETICA_18);
iText(895,screenHeight-90,"  Font 6",GLUT_BITMAP_TIMES_ROMAN_10);
iText(970,screenHeight-95,"Font 7",GLUT_BITMAP_TIMES_ROMAN_24);


iFilledRectangle(1050,screenHeight-75,25,25);

iText(1050,screenHeight-90,"Dynamic Circle",GLUT_BITMAP_9_BY_15);




iFilledRectangle(1250,screenHeight-90,25,25);
iText(1280,screenHeight-85,"Save",GLUT_BITMAP_8_BY_13);






  }

  else if(mode==3)
  {

       iShowBMP(39,0,"Image//home_screen.bmp");
       iSetColor(255,200,150);
       iText(39,750,"Brush and Eraser tools have 5 different widths available for use.A rich color palette of 48 different is available for all the tools.",GLUT_BITMAP_TIMES_ROMAN_24);
       iText(39,720,"Circle,Ellipse,Rectangle can be used as filled or border shape.Five different sizes can be selected by the 'Tool Width' tool.",GLUT_BITMAP_TIMES_ROMAN_24);

       iText(39,690,"Text can be written in all 7 fonts and before pressing ENTER, it can be placed elsewhere on the canvas if needed.",GLUT_BITMAP_TIMES_ROMAN_24);

       iText(39,660,"The line tool works with 5 different widths and it draws a line between any two points on canvas that is clicked by the user.",GLUT_BITMAP_TIMES_ROMAN_24);

       iText(39,630,"The dynamic circle tool allows a user to draw any filled or border circle between any two points on canvas that is clicked by the user.",GLUT_BITMAP_TIMES_ROMAN_24);

       iText(39,600,"Select the Save tool and then write name of file to be saved,then finally press ENTER to save the file.",GLUT_BITMAP_TIMES_ROMAN_24);



        iText(39,570,"END: Closes the application",GLUT_BITMAP_TIMES_ROMAN_24);

        iText(39,540,"PAGE DOWN: Pressing this while on drawing screen will clear the screen for fresh drawing.",GLUT_BITMAP_TIMES_ROMAN_24);

        iText(39,510,"HOME: Returns to main menu.",GLUT_BITMAP_TIMES_ROMAN_24);

        iText(39,480,"For loading: View Drawing - Select any previously save drawing - HOME - Start drawing ",GLUT_BITMAP_TIMES_ROMAN_24);





  }










  else if(/*mode==3||*/mode==4)
  {


      for (int i=0; i<save_cnt; i++)
        {

                iSetColor(255,123,192);
                iFilledRectangle(80, 550-40*i, 200, 30);

                iSetColor(255, 255, 255);
                iText(90, 560-40*i, save_names[i], GLUT_BITMAP_HELVETICA_18);

            }





  }



  else if(mode==5)
  {

      show_things();


  }











}





void iMouseMove(int mx, int my) {
	//printf("x = %d, y= %d\n",mx,my);
	//place your codes here

	if(tool==1&&mode==2)
    {


	PointsArray[total_points].x=mx;
       PointsArray[total_points].y=my;
       PointsArray[total_points].r=curr_red;
       PointsArray[total_points].g=curr_green;
       PointsArray[total_points].b=curr_blue;
       PointsArray[total_points].size=curr_points_size;
       total_points++;


    }

}








/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/


/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my)
{

    if(mode==1)
    {
        if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=500+150&&mx<=700+150&&my>=300+63&&my<=350+63)
        {
            mode=2;
            return;
        }


        if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=500+150&&mx<=700+150&&my>=200+63&&my<=250+63)
        {
            //readList();//
            mode=3;

        }


        if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=500+150&&mx<=700+150&&my>=100+63&&my<=150+63)
        {

            readList();
            mode=4;

        }








    }




else if(mode==2)
{


      if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&my>=screenHeight-25)
  {








  if(tool==1)

   {


      if(((mx-1325)*(mx-1325)+(my-screenHeight+15)*(my-screenHeight+15))<=5*5) curr_points_size=5;
      if(((mx-1360)*(mx-1360)+(my-screenHeight+15)*(my-screenHeight+15))<=7*7) curr_points_size=7;
      if(((mx-1395)*(mx-1395)+(my-screenHeight+15)*(my-screenHeight+15))<=9*9) curr_points_size=9;
      if(((mx-1430)*(mx-1430)+(my-screenHeight+15)*(my-screenHeight+15))<=11*11) curr_points_size=11;
      if(((mx-1465)*(mx-1465)+(my-screenHeight+15)*(my-screenHeight+15))<=13*13) curr_points_size=13;






   }


   if(tool==2)
   {

       if(((mx-1325)*(mx-1325)+(my-screenHeight+15)*(my-screenHeight+15))<=5*5) curr_circles_size=50;
      if(((mx-1360)*(mx-1360)+(my-screenHeight+15)*(my-screenHeight+15))<=7*7) curr_circles_size=70;
      if(((mx-1395)*(mx-1395)+(my-screenHeight+15)*(my-screenHeight+15))<=9*9) curr_circles_size=90;
      if(((mx-1430)*(mx-1430)+(my-screenHeight+15)*(my-screenHeight+15))<=11*11) curr_circles_size=110;
      if(((mx-1465)*(mx-1465)+(my-screenHeight+15)*(my-screenHeight+15))<=13*13) curr_circles_size=130;







   }

   if(tool==3)
   {

       if(((mx-1325)*(mx-1325)+(my-screenHeight+15)*(my-screenHeight+15))<=5*5) curr_rect_length=100,curr_rect_width=50;
      if(((mx-1360)*(mx-1360)+(my-screenHeight+15)*(my-screenHeight+15))<=7*7) curr_rect_length=150,curr_rect_width=75;
      if(((mx-1395)*(mx-1395)+(my-screenHeight+15)*(my-screenHeight+15))<=9*9) curr_rect_length=200,curr_rect_width=100;
      if(((mx-1430)*(mx-1430)+(my-screenHeight+15)*(my-screenHeight+15))<=11*11) curr_rect_length=250,curr_rect_width=125;
      if(((mx-1465)*(mx-1465)+(my-screenHeight+15)*(my-screenHeight+15))<=13*13) curr_rect_length=300,curr_rect_width=150;







   }

   if(tool==7)
   {

       if(((mx-1325)*(mx-1325)+(my-screenHeight+15)*(my-screenHeight+15))<=5*5) curr_major=50,curr_minor=25;
      if(((mx-1360)*(mx-1360)+(my-screenHeight+15)*(my-screenHeight+15))<=7*7) curr_major=100,curr_minor=50;
      if(((mx-1395)*(mx-1395)+(my-screenHeight+15)*(my-screenHeight+15))<=9*9) curr_major=150,curr_minor=75;
      if(((mx-1430)*(mx-1430)+(my-screenHeight+15)*(my-screenHeight+15))<=11*11) curr_major=180,curr_minor=90;
      if(((mx-1465)*(mx-1465)+(my-screenHeight+15)*(my-screenHeight+15))<=13*13) curr_major=200,curr_minor=100;








   }


   if(tool==4)

{


    if(((mx-1325)*(mx-1325)+(my-screenHeight+15)*(my-screenHeight+15))<=5*5)
       {

        curr_line_width=5;
        return;

       }

      if(((mx-1360)*(mx-1360)+(my-screenHeight+15)*(my-screenHeight+15))<=7*7)
        {

        curr_line_width=15;
        return;

       }
      if(((mx-1395)*(mx-1395)+(my-screenHeight+15)*(my-screenHeight+15))<=9*9)
      {

        curr_line_width=25;
        return;

       }

      if(((mx-1430)*(mx-1430)+(my-screenHeight+15)*(my-screenHeight+15))<=11*11)
        {

        curr_line_width=35;
        return;

       }
      if(((mx-1465)*(mx-1465)+(my-screenHeight+15)*(my-screenHeight+15))<=13*13)
        {

        curr_line_width=45;
        return;

       }










}














  }










  if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&my>=screenHeight-25&&mx<=1200)
   {

       iGetPixelColor(mx,my,pixelColor);
       curr_red=pixelColor[0];
       curr_green=pixelColor[1];
       curr_blue=pixelColor[2];

     //tool=1;//
      if(tool==4||tool==8) return;



   }

   if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=1200&&mx<=1225&&my<=screenHeight&&my>=screenHeight-25)//
  {

   tool=1;



  }


  if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&my>=screenHeight-25&&mx>=1250&&mx<=1275)  //Eraser
   {


       curr_red=0;
       curr_green=0;
       curr_blue=0;
       tool=1;//





   }





  if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=0&&mx<=25&&my<=screenHeight-50&&my>=screenHeight-75)//
  {


  tool=2;




}



if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=75&&mx<=100&&my<=screenHeight-25&&my>=screenHeight-75)//
  {

   tool=3;

  }


  if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=225&&mx<=250&&my<=screenHeight-50&&my>=screenHeight-75)//
  {


  tool=6;



}


   if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=300&&mx<=325&&my<=screenHeight-50&&my>=screenHeight-75)//
  {


  tool=7;



}

 if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=375&&mx<=400&&my<=screenHeight-50&&my>=screenHeight-75)//
  {


  curr_filled=1;
  if(tool==8) return;



}


if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=450&&mx<=475&&my<=screenHeight-50&&my>=screenHeight-75)//
  {


  curr_filled=0;
if(tool==8) return;




}


if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=525&&mx<=550&&my<=screenHeight-50&&my>=screenHeight-75)
  {

      curr_font=1;


  }



if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=600&&mx<=625&&my<=screenHeight-50&&my>=screenHeight-75)
  {

      curr_font=2;


  }

if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=675&&mx<=700&&my<=screenHeight-50&&my>=screenHeight-75)
  {

      curr_font=3;


  }

if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=750&&mx<=775&&my<=screenHeight-50&&my>=screenHeight-75)
  {

      curr_font=4;


  }


  if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=825&&mx<=850&&my<=screenHeight-50&&my>=screenHeight-75)
  {

      curr_font=5;


  }



  if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=900&&mx<=925&&my<=screenHeight-50&&my>=screenHeight-75)
  {

      curr_font=6;


  }


  if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=975&&mx<=1000&&my<=screenHeight-50&&my>=screenHeight-75)
  {

      curr_font=7;


  }




  if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=1050&&mx<=1075&&my<=screenHeight-50&&my>=screenHeight-75)
  {

      tool=8;
      return;


  }




  if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=150&&mx<=175&&my<=screenHeight-50&&my>=screenHeight-75)//
  {


  tool=4;
  return;//



}


   if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=1250&&mx<=1275&&my<=screenHeight-65&&my>=screenHeight-90)
   {
       tool=10;
       printf("yes");//
       return;
   }



  if(tool==10&&button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&mx>=1250&&mx<=1275&&my<=screenHeight-65&&my>=screenHeight-90)//
  {

     printf("/nEnter save file name:\n");
      len=0;
      tool=1;
      SaveFile(save_filename);
      return;




  }






   if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&tool==1)
   {

       PointsArray[total_points].x=mx;
       PointsArray[total_points].y=my;
       PointsArray[total_points].r=curr_red;
       PointsArray[total_points].g=curr_green;
       PointsArray[total_points].b=curr_blue;
       PointsArray[total_points].size=curr_points_size;
       total_points++;

      //freeDraw=1;



   }

   if(tool==2&&button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
   {

      circlesAt[total_circles]=total_points++;
      CirclesArray[total_circles].x=mx;
       CirclesArray[total_circles].y=my;
       CirclesArray[total_circles].r=curr_red;
       CirclesArray[total_circles].g=curr_green;
       CirclesArray[total_circles].b=curr_blue;
       CirclesArray[total_circles].size=curr_circles_size;
       CirclesArray[total_circles].filled=curr_filled;
       total_circles++;




   }


   if(tool==3&&button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
   {

      rectanglesAt[total_rect]=total_points++;
     RectanglesArray[total_rect].x=mx;
       RectanglesArray[total_rect].y=my;
       RectanglesArray[total_rect].r=curr_red;
       RectanglesArray[total_rect].g=curr_green;
       RectanglesArray[total_rect].b=curr_blue;
       RectanglesArray[total_rect].dx=curr_rect_length;
       RectanglesArray[total_rect].dy=curr_rect_width;
       RectanglesArray[total_rect].filled=curr_filled;


       total_rect++;




   }

if(tool==4&&button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
   {


     LinesArray[total_lines].x1=mx;
       LinesArray[total_lines].y1=my;
       LinesArray[total_lines].r=curr_red;
       LinesArray[total_lines].g=curr_green;
       LinesArray[total_lines].b=curr_blue;
       LinesArray[total_lines].width=curr_line_width;

      tool=5;
      return;


   }


   if(tool==5&&button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
   {




     LinesArray[total_lines].x2=mx;
       LinesArray[total_lines].y2=my;
       linesAt[total_lines]=total_points++;



     tool=4;
     total_lines++;




   }


   if(tool==8&&button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
   {


     DynamicCirclesArray[total_dynamic_circles].x1=mx;
     DynamicCirclesArray[total_dynamic_circles].y1=my;
      DynamicCirclesArray[total_dynamic_circles].r=curr_red;
       DynamicCirclesArray[total_dynamic_circles].g=curr_green;
    DynamicCirclesArray[total_dynamic_circles].b=curr_blue;
    DynamicCirclesArray[total_dynamic_circles].filled=curr_filled;




     tool=9;
      return;


   }


   if(tool==9&&button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
   {




      DynamicCirclesArray[total_dynamic_circles].x2=mx;
        DynamicCirclesArray[total_dynamic_circles].y2=my;
       dynamic_circlesAt[total_dynamic_circles]=total_points++;



     tool=8;
     total_dynamic_circles++;




   }













   if(tool==6&&button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
   {

       textAt[total_text]=total_points++;
       TextArray[total_text].r=curr_red;
       TextArray[total_text].g=curr_green;
       TextArray[total_text].b=curr_blue;
       TextArray[total_text].x=mx;
       TextArray[total_text].y=my;
       TextArray[total_text].font=curr_font;



       /*fflush(stdin);

       gets(TextArray[total_text].text);

       fflush(stdin);*/






   }






    /*if (tool==6 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {      // circle tool

            circlesAt[circleCnt]= total_pts++;                              // total_pts is actually total items/things
            circleArr[circleCnt].x= mx;                                        // at this total_pts, there is a circle, store this total_pts in arr, and increment it
            circleArr[circleCnt].y= my;
            circleArr[circleCnt].r= curr_r;
            circleArr[circleCnt].g= curr_g;
            circleArr[circleCnt].b= curr_b;
            circleArr[circleCnt].size= curr_pts_size+28;

            circleCnt++;

        }*/






   /*if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&my>=screenHeight-25&&mx<=1200)
   {

       iGetPixelColor(mx,my,pixelColor);
       curr_red=pixelColor[0];
       curr_green=pixelColor[1];
       curr_blue=pixelColor[2];

     //tool=1;//




   }*/

if(tool==7&&button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
   {

      ellipsesAt[total_ellipses]=total_points++;
      EllipsesArray[total_ellipses].x=mx;
      EllipsesArray[total_ellipses].y=my;
      EllipsesArray[total_ellipses].r=curr_red;
      EllipsesArray[total_ellipses].g=curr_green;
      EllipsesArray[total_ellipses].b=curr_blue;
      EllipsesArray[total_ellipses].da=curr_major;
      EllipsesArray[total_ellipses].db=curr_minor;
      EllipsesArray[total_ellipses].filled=curr_filled;



      total_ellipses++;






   }


//tool options:

/*if(tool==1&&clear_screen==1&&button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
{

    clear_screen=0;
    //curr_points_size=9;



}*/





if(tool==11&&button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN/*&&clear_screen==0*/)
{
    tool=1;//brush


    rectanglesAt[total_rect]=total_points++;
     RectanglesArray[total_rect].x=0;
       RectanglesArray[total_rect].y=0;
       RectanglesArray[total_rect].r=0;
       RectanglesArray[total_rect].g=0;
       RectanglesArray[total_rect].b=0;
       RectanglesArray[total_rect].dx=screenWidth;
       RectanglesArray[total_rect].dy=canvasHeight;
       RectanglesArray[total_rect].filled=1;


       total_rect++;
    //clear_screen=1;
    return;


}














}


else if (/*mode==3 ||*/ mode==4)
    {              // load screen



    if ( mx>= 80 && mx <= 280 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

            int ind= (580-my)/40;
            if (ind < save_cnt) {
                LoadFile(save_names[ind]);
               //if (mode==3) mode =2;//
               if (mode==4) mode =5;
                }
             }






    }



else if(mode==5)
{

   show_things();


}











  }//














/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key)
{
	if (key == 'q')

    {
		exit(0);
	}






	if(tool==6)
    {

        if(key=='\r') TextArray[total_text++].text[len]='\0',tool=6,len=0;//ending string for carriage return / ENTER

        else

        {

            if(len>=0&&len<=8)
            {

                TextArray[total_text].text[len++]=key;


            }

            else

            TextArray[total_text++].text[len++]='\0',tool=6,len=0;


        }






    }




if (tool==10 && mode==2) {                            // taking input for savefile name

       if (key=='\r')  len=0, tool=1,  SaveFile(save_filename);

       else {
            if (key==8 && len >=0)  save_filename[len--]='\0';
            else save_filename[len++]=key;
       }
    }









	//place your codes for other keys here
}


void iSpecialKeyboard(unsigned char key) {

	if (key == GLUT_KEY_END) {
		exit(0);
	}

if(key==GLUT_KEY_HOME)
    {
        mode=1;
    }


if(key==GLUT_KEY_HOME&&mode==2)
{

    //mode=1;
    init();
    freeMem();
    erase();
    mode=1;

}


if(key==GLUT_KEY_PAGE_DOWN&&mode==2)//
{

    tool=11;//
    //printf("DONE\n");
    //last_point_index=total_points;//
    //printf("%d",total_points);


}//








}




int main()
{
    mode=1;//
    init();//


	/*if(mode==1) iInitialize(1200,675, "Digital Drawing Board");
	else*/ iInitialize(screenWidth,screenHeight, "Digital Drawing Board");
	freeMem();
	return 0;
}


void init() {

    /*total_points=0;
    textsAtCnt=0; len=0, save_cnt=0, circleCnt=0, filledCircleCnt=0, rectangleCnt=0, filledRectCnt=0,
    ellipseCnt=0, filledEllipseCnt=0, lineCnt=0, polyCnt=0, filledPolyCnt=0;
    curr_r=0, curr_g=0, curr_b=0, curr_pts_size=1;
    tool=0, showingMoreIcons=0;
    showPrompt=0, showPallete=0, showSavePrompt=0;
    promptMsgNo=0;
    threeSecCnt=0;*/

    int pixelColor[3];
int total_points=0,total_circles=0,total_rect=0,total_lines=0,total_text=0,total_ellipses=0,total_dynamic_circles=0;
int curr_red,curr_blue,curr_green;
int len=0;
int curr_points_size=7,curr_circles_size=50,curr_rect_length=100,curr_rect_width=50,curr_major=50,curr_minor=25;
int curr_line_width=25;//arbitrary
int curr_font=1;//1-7 for fonts

int curr_filled=1;

int tool_width=2;//

int mode=1;
//Mode 1 for homescreen, Mode 2 for drawing board

int tool=1;// tool=1 for free draw and eraser,tool=2 for circle,tool=3 for rectangle,tool=4,5 for start end points of a line,tool=6 for text,tool=7 for ellipse
//tools 8,9 for dynamic circle

int save_cnt=0;










}








void freeMem() {

    free(PointsArray);
    free(TextArray);
    free(textAt);
    free(CirclesArray);
    free(circlesAt);
    free(DynamicCirclesArray);
    free(dynamic_circlesAt);
    free(rectanglesAt);
    free(RectanglesArray);

    free(ellipsesAt);
    free(EllipsesArray);

    free(linesAt);
    free(LinesArray);


}



void erase()
{

    PointsArray= (points *) malloc(9999999 * sizeof(points));


CirclesArray= (circles *) malloc(100 * sizeof(circles));
circlesAt=(int *)malloc(100*sizeof(int));

RectanglesArray= (rectangles *) malloc(100 * sizeof(rectangles));
 rectanglesAt=(int *)malloc(100*sizeof(int));


LinesArray=(lines *)malloc(100 * sizeof(lines));
linesAt=(int *)malloc(100*sizeof(int));


TextArray=(textbox*)malloc(10*sizeof(textbox));
textAt=(int *)malloc(10*sizeof(int));

EllipsesArray=(ellipses*)malloc(100*sizeof(ellipses));
ellipsesAt=(int *) malloc(sizeof(int)*100);



DynamicCirclesArray= (dynamic_circles*) malloc(100*sizeof(dynamic_circles));//
dynamic_circlesAt=(int *)malloc(sizeof(int)*100);//






}





