#include <qtvcf.h>

N::VcfColorSelector:: VcfColorSelector ( VcfHeadParaments  )
                    : VcfPicture       ( parent , item , p )
                    , direction        ( 0                 )
                    , colorModel       ( 0                 )
                    , color            ( NULL              )
{
}

N::VcfColorSelector::~VcfColorSelector (void)
{
}

void N::VcfColorSelector::paint(QPainter * painter,const QStyleOptionGraphicsItem * option,QWidget * widget)
{
  Paint(painter,ScreenRect,false,true);
}

QVariant N::VcfColorSelector::itemChange(GraphicsItemChange change,const QVariant & value)
{
  switch (change)                                       {
    case ItemPositionChange                             :
    case ItemPositionHasChanged                         :
      if (NotNull(Options))                             {
        QPointF scenePos = value.toPointF(  )           ;
      }                                                 ;
    break                                               ;
    case ItemSelectedHasChanged                         :
      DeleteGadgets()                                   ;
    break                                               ;
  }                                                     ;
  return QGraphicsItem::itemChange(change,value)        ;
}

void N::VcfColorSelector::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  scalePressEvent ( event )                            ;
  if (IsNull(color)) return                            ;
  if (!IsMask(event->buttons(),Qt::LeftButton)) return ;
  QSize   is = Image.size()                            ;
  QPointF p  = event->pos()                            ;
  p -= ScreenRect.topLeft()                            ;
  QPoint  P  = p.toPoint ()                            ;
  if (is.width ()<P.x()) return                        ;
  if (is.height()<P.y()) return                        ;
  unsigned char * sl                                   ;
  sl = (unsigned char *)Image.constScanLine(P.y())     ;
  sl = &sl[ 4 * P.x() ]                                ;
  color->setBlue  (sl[0])                              ;
  color->setGreen (sl[1])                              ;
  color->setRed   (sl[2])                              ;
  color->setAlpha (sl[3])                              ;
  QString Q                                            ;
  QColor  C = *color                                   ;
  Q = tr("[%1,%2,%3,%4]"                               )
      .arg(C.red   ()                                  )
      .arg(C.green ()                                  )
      .arg(C.blue  ()                                  )
      .arg(C.alpha ()                                ) ;
  P = QCursor::pos ()                                  ;
  QToolTip::showText ( P , Q )                         ;
  DoProcessEvents                                      ;
  emit Update ( this )                                 ;
}

void N::VcfColorSelector::setSize(int width,int height)
{
  Image = QImage ( QSize ( width , height ) , QImage::Format_ARGB32 ) ;
}

void N::VcfColorSelector::setSize(QSize & size)
{
  Image = QImage(size,QImage::Format_ARGB32) ;
}

QColor N::VcfColorSelector::Yuv(double Y,double U,double V)
{
  return ColorSpace :: YUV ( Y , U , V ) ;
}

QColor N::VcfColorSelector::YIQ(double Y,double I,double Q)
{
  return ColorSpace :: YIQ ( Y , I , Q ) ;
}

QColor N::VcfColorSelector::Lab(double L,double a,double b)
{
  return ColorSpace :: Lab ( L , a , b ) ;
}

QColor N::VcfColorSelector::Luv(double L,double u,double v)
{
  return ColorSpace :: LUV ( L , u , v ) ;
}

QColor N::VcfColorSelector::XYZ(double X,double Y,double Z)
{
  return ColorSpace :: XYZ ( X , Y , Z ) ;
}

void N::VcfColorSelector::setR(int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  QColor D (255,255,255)         ;
  nIfSafe(color) D = *color      ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        QColor C                 ;
        double R = dh - y        ;
        R /= dh                  ;
        C  = D                   ;
        C . setRedF   ( R )      ;
        QRgb P = C.rgba ( )      ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        QColor C                 ;
        double R = x             ;
        R /= dw                  ;
        C  = D                   ;
        C . setRedF   ( R )      ;
        QRgb P = C.rgba ( )      ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setG(int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  QColor D (255,255,255)         ;
  nIfSafe(color) D = *color      ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        QColor C                 ;
        double G = dh - y        ;
        G /= dh                  ;
        C  = D                   ;
        C . setGreenF ( G )      ;
        QRgb P = C.rgba ( )      ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        QColor C                 ;
        double G = x             ;
        G /= dw                  ;
        C  = D                   ;
        C . setGreenF ( G )      ;
        QRgb P = C.rgba ( )      ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setB(int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  QColor D (255,255,255)         ;
  nIfSafe(color) D = *color      ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        QColor C                 ;
        double B = dh - y        ;
        B /= dh                  ;
        C  = D                   ;
        C . setBlueF  ( B )      ;
        QRgb P = C.rgba ( )      ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        QColor C                 ;
        double B = x             ;
        B /= dw                  ;
        C  = D                   ;
        C . setBlueF  ( B )      ;
        QRgb P = C.rgba ( )      ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setA(int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  QColor D (255,255,255)         ;
  nIfSafe(color) D = *color      ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        QColor C                 ;
        double A = dh - y        ;
        A /= dh                  ;
        C  = D                   ;
        C . setAlphaF ( A )      ;
        QRgb P = C.rgba ( )      ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        QColor C                 ;
        double A = x             ;
        A /= dw                  ;
        C  = D                   ;
        C . setAlphaF ( A )      ;
        QRgb P = C.rgba ( )      ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setC(int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  QColor D (255,255,255)         ;
  nIfSafe(color) D = *color      ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        QColor C                 ;
        double R = dh - y        ;
        R /= dh                  ;
        C . setCmyk (R,D.magentaF(),D.yellowF(),D.blackF()) ;
        QRgb P = C.rgba ( )      ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        QColor C                 ;
        double R = x             ;
        R /= dw                  ;
        C . setCmykF (R,D.magentaF(),D.yellowF(),D.blackF()) ;
        QRgb P = C.rgba ( )      ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setY(int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  QColor D (255,255,255)         ;
  nIfSafe(color) D = *color      ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        QColor C                 ;
        double R = dh - y        ;
        R /= dh                  ;
        C . setCmyk (D.cyanF(),D.magentaF(),R,D.blackF()) ;
        QRgb P = C.rgba ( )      ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        QColor C                 ;
        double R = x             ;
        R /= dw                  ;
        C . setCmyk (D.cyanF(),D.magentaF(),R,D.blackF()) ;
        QRgb P = C.rgba ( )      ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setM(int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  QColor D (255,255,255)         ;
  nIfSafe(color) D = *color      ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        QColor C                 ;
        double R = dh - y        ;
        R /= dh                  ;
        C . setCmyk (D.cyanF(),R,D.yellowF(),D.blackF()) ;
        QRgb P = C.rgba ( )      ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        QColor C                 ;
        double R = x             ;
        R /= dw                  ;
        C . setCmykF (D.cyanF(),R,D.yellowF(),D.blackF()) ;
        QRgb P = C.rgba ( )      ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setK(int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  QColor D (255,255,255)         ;
  nIfSafe(color) D = *color      ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        QColor C                 ;
        double R = dh - y        ;
        R /= dh                  ;
        C . setCmykF (D.cyanF(),D.magentaF(),D.yellowF(),R) ;
        QRgb P = C.rgba ( )      ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        QColor C                 ;
        double R = x             ;
        R /= dw                  ;
        C . setCmykF (D.cyanF(),D.magentaF(),D.yellowF(),R) ;
        QRgb P = C.rgba ( )      ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setHSV(double V)
{
  QSize s  = Image . size   ( ) ;
  int   w  = s     . width  ( ) ;
  int   h  = s     . height ( ) ;
  int   x                       ;
  int   y                       ;
  int   dw = w - 1              ;
  int   dh = h - 1              ;
  for (y=0;y<h;y++)             {
    double S = dh - y           ;
    S /= dh                     ;
    for (x=0;x<w;x++)           {
      double H = dw - x         ;
      H /= dw                   ;
      QColor C                  ;
      C . setHsvF(H,S,V)        ;
      QRgb   P = C.rgba ()      ;
      Image.setPixel(x,y,P)     ;
    }                           ;
  }                             ;
}

void N::VcfColorSelector::setHsvV(int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  QColor D (255,255,255)         ;
  nIfSafe(color) D = *color      ;
  double H = D.hueF        ()    ;
  double S = D.saturationF ()    ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        QColor C                 ;
        double V = dh - y        ;
        V /= dh                  ;
        C  = D                   ;
        C . setHsvF ( H,S,V )    ;
        QRgb P = C.rgba ( )      ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        QColor C                 ;
        double V = x             ;
        V /= dw                  ;
        C  = D                   ;
        C . setHsvF ( H,S,V )    ;
        QRgb P = C.rgba ( )      ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setHSL(double L)
{
  QSize s  = Image . size   ( ) ;
  int   w  = s     . width  ( ) ;
  int   h  = s     . height ( ) ;
  int   x                       ;
  int   y                       ;
  int   dw = w - 1              ;
  int   dh = h - 1              ;
  for (y=0;y<h;y++)             {
    double S = dh - y           ;
    S /= dh                     ;
    for (x=0;x<w;x++)           {
      double H = dw - x         ;
      H /= dw                   ;
      QColor C                  ;
      C . setHslF(H,S,L)        ;
      QRgb   P = C.rgba ()      ;
      Image.setPixel(x,y,P)     ;
    }                           ;
  }                             ;
}

void N::VcfColorSelector::setHslL(int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  QColor D (255,255,255)         ;
  nIfSafe(color) D = *color      ;
  double H = D.hueF        ()    ;
  double S = D.saturationF ()    ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        QColor C                 ;
        double L = dh - y        ;
        L /= dh                  ;
        C  = D                   ;
        C . setHslF ( H,S,L )    ;
        QRgb P = C.rgba ( )      ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        QColor C                 ;
        double L = x             ;
        L /= dw                  ;
        C  = D                   ;
        C . setHslF ( H,S,L )    ;
        QRgb P = C.rgba ( )      ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setYUV(double Y)
{
  QSize s  = Image . size   ( ) ;
  int   w  = s     . width  ( ) ;
  int   h  = s     . height ( ) ;
  int   x                       ;
  int   y                       ;
  int   dw = w - 1              ;
  int   dh = h - 1              ;
  for (y=0;y<h;y++)             {
    double V = dh - y           ;
    V /= dh                     ;
    V -= 0.500                  ;
    V *=  1.230                 ;
    for (x=0;x<w;x++)           {
      double U = x              ;
      U /= dw                   ;
      U -= 0.500                ;
      U *= 0.872                ;
      QColor C = Yuv(Y,U,V)     ;
      QRgb   P = C.rgba ()      ;
      Image.setPixel(x,y,P)     ;
    }                           ;
  }                             ;
}

void N::VcfColorSelector::setYuvY(double U,double V,int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        double Y = dh - y        ;
        Y /= dh                  ;
        QColor C = Yuv(Y,U,V)    ;
        QRgb   P = C.rgba ( )    ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        double Y = x             ;
        Y /= dw                  ;
        QColor C = Yuv(Y,U,V)    ;
        QRgb   P = C.rgba ( )    ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setYIQ(double Y)
{
  QSize s  = Image . size   ( ) ;
  int   w  = s     . width  ( ) ;
  int   h  = s     . height ( ) ;
  int   x                       ;
  int   y                       ;
  int   dw = w - 1              ;
  int   dh = h - 1              ;
  for (y=0;y<h;y++)             {
    double Q = dh - y           ;
    Q /= dh                     ;
    Q -= 0.500                  ;
    Q *= 1.045182               ;
    for (x=0;x<w;x++)           {
      double I = x              ;
      I /= dw                   ;
      I -= 0.500                ;
      I *= 1.191432             ;
      QColor C = YIQ(Y,I,Q)     ;
      QRgb   P = C.rgba ()      ;
      Image.setPixel(x,y,P)     ;
    }                           ;
  }                             ;
}

void N::VcfColorSelector::setYiqY(double I,double Q,int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        double Y = dh - y        ;
        Y /= dh                  ;
        QColor C = YIQ(Y,I,Q)    ;
        QRgb   P = C.rgba ( )    ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        double Y = x             ;
        Y /= dw                  ;
        QColor C = YIQ(Y,I,Q)    ;
        QRgb   P = C.rgba ( )    ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setLab(double L)
{
  QSize s  = Image . size   ( )  ;
  int   w  = s     . width  ( )  ;
  int   h  = s     . height ( )  ;
  int   x                        ;
  int   y                        ;
  int   dw = w - 1               ;
  int   dh = h - 1               ;
  for (y=0;y<h;y++)              {
    double b = dh - y            ;
    b /= dh                      ;
    b *= ( 107.8640 + 94.4825 )  ;
    b -=   107.8640              ;
    for (x=0;x<w;x++)            {
      double a = x               ;
      a /= dw                    ;
      a *= ( 86.1846 + 98.2542 ) ;
      a -=   86.1846             ;
      QColor C = Lab(L,a,b)      ;
      QRgb   P = C.rgba ()       ;
      Image.setPixel(x,y,P)      ;
    }                            ;
  }                              ;
}

void N::VcfColorSelector::setLabL(double a,double b,int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        QColor C                 ;
        double L = dh - y        ;
        L /= dh                  ;
        L *= 100.0               ;
        C  = Lab ( L , a , b )   ;
        QRgb P = C.rgba ( )      ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        QColor C                 ;
        double L = x             ;
        L /= dw                  ;
        L *= 100.0               ;
        C  = Lab ( L , a , b )   ;
        QRgb P = C.rgba ( )      ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setXYZ(double Y)
{
  QSize s  = Image . size   ( )  ;
  int   w  = s     . width  ( )  ;
  int   h  = s     . height ( )  ;
  int   x                        ;
  int   y                        ;
  int   dw = w - 1               ;
  int   dh = h - 1               ;
  for (y=0;y<h;y++)              {
    double Z = dh - y            ;
    Z /= dh                      ;
    Z *= 1.08883                 ;
    for (x=0;x<w;x++)            {
      double X = x               ;
      X /= dw                    ;
      X *= 0.95047               ;
      QColor C = XYZ(X,Y,Z)      ;
      QRgb   P = C.rgba ()       ;
      Image.setPixel(x,y,P)      ;
    }                            ;
  }                              ;
}

void N::VcfColorSelector::setXyzY(double X,double Z,int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        QColor C                 ;
        double Y = dh - y        ;
        Y /= dh                  ;
        C  = XYZ ( X , Y , Z )   ;
        QRgb P = C.rgba ( )      ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        QColor C                 ;
        double Y = x             ;
        Y /= dw                  ;
        C  = XYZ ( X , Y , Z )   ;
        QRgb P = C.rgba ( )      ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}

void N::VcfColorSelector::setLuv(double L)
{
  QSize s  = Image . size   ( ) ;
  int   w  = s     . width  ( ) ;
  int   h  = s     . height ( ) ;
  int   x                       ;
  int   y                       ;
  int   dw = w - 1              ;
  int   dh = h - 1              ;
  for (y=0;y<h;y++)             {
    double v = dh - y           ;
    v /= dh                     ;
    v *= ( 21.0972 + 435.3520 ) ;
    v -=   21.0972              ;
    for (x=0;x<w;x++)           {
      double u = x              ;
      u /= dw                   ;
      u *= ( 83.0798+175.0530 ) ;
      u -=   83.0798            ;
      QColor C = Luv(L,u,v)     ;
      QRgb   P = C.rgba ()      ;
      Image.setPixel(x,y,P)     ;
    }                           ;
  }                             ;
}

void N::VcfColorSelector::setLuvL(double u,double v,int direction)
{
  QSize  s  = Image . size   ( ) ;
  int    w  = s     . width  ( ) ;
  int    h  = s     . height ( ) ;
  int    x                       ;
  int    y                       ;
  int    dw = w - 1              ;
  int    dh = h - 1              ;
  switch (direction)             {
    case Qt::Vertical            :
      for (y=0;y<h;y++)          {
        QColor C                 ;
        double L = dh - y        ;
        L /= dh                  ;
        L *= 100.0               ;
        C  = Luv ( L , u , v )   ;
        QRgb P = C.rgba ( )      ;
        for (x=0;x<w;x++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
    case Qt::Horizontal          :
      for (x=0;x<w;x++)          {
        QColor C                 ;
        double L = x             ;
        L /= dw                  ;
        L *= 100.0               ;
        C  = Luv ( L , u , v )   ;
        QRgb P = C.rgba ( )      ;
        for (y=0;y<h;y++)        {
          Image.setPixel(x,y,P)  ;
        }                        ;
      }                          ;
    break                        ;
  }                              ;
}
