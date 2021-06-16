#include <qtvcf.h>

N::VcfScreenSize:: VcfScreenSize ( VcfHeadParaments  )
                 : VcfRectangle  ( parent , item , p )
                 , screen        ( NULL              )
{
}

N::VcfScreenSize::~VcfScreenSize (void)
{
}

QPainterPath N::VcfScreenSize::shape(void) const
{
  QPainterPath path          ;
  path . addRect(ScreenRect) ;
  return path                ;
}

void N::VcfScreenSize::paint(QPainter * painter,const QStyleOptionGraphicsItem * option,QWidget * widget)
{
  Paint ( painter , ScreenRect , true , true ) ;
}

void N::VcfScreenSize::Paint(QPainter * p,QRectF Region,bool clip,bool color)
{
  p -> setPen    ( QPen  (QColor(160,160,160)) ) ;
  p -> setBrush  ( brush (Region             ) ) ;
  p -> drawRect  ( Region                      ) ;
  drawVertical   ( p , Region                  ) ;
  drawHorizontal ( p , Region                  ) ;
  PaintImage     ( p , Region                  ) ;
}

void N::VcfScreenSize::drawVertical(QPainter * p,QRectF Region)
{
  QRectF R = Region                               ;
  qreal  L = R.left   () + ((R.width()*3)/4) - 20 ;
  qreal  V = L + 80                               ;
  int    i = 0                                    ;
  double y = 0                                    ;
  QPen   PEN ( QColor ( 255 , 0 , 0 ) )           ;
  PEN . setWidthF ( 1.0 )                         ;
  p  -> setPen    ( PEN )                         ;
  do                                              {
    y = i                                         ;
    QPointF P (0,y/10)                            ;
    P  = Options->position(P)                     ;
    y  = P.y()                                    ;
    if (y<ScreenRect.height())                    {
      qreal xp                                    ;
      if ((i%10)==0) xp = L                  ; else
      if ((i% 5)==0) xp = L + 20             ; else
                     xp = L + 40                  ;
      p -> drawLine ( QLineF ( xp , y , V , y ) ) ;
    }                                             ;
    i++                                           ;
  } while (y<ScreenRect.height())                 ;
}

void N::VcfScreenSize::drawHorizontal(QPainter * p,QRectF Region)
{
  QRectF R = Region                               ;
  qreal  T = R.top   () + ((R.height()*3)/4) - 20 ;
  qreal  V = T + 80                               ;
  int    i = 0                                    ;
  double x = 0                                    ;
  QPen   PEN ( QColor ( 0 , 0 , 255 ) )           ;
  PEN . setWidthF ( 1.0 )                         ;
  p  -> setPen    ( PEN )                         ;
  do                                              {
    x = i                                         ;
    QPointF P (x/10,0)                            ;
    P  = Options->position(P)                     ;
    x  = P.x()                                    ;
    if (x<ScreenRect.width())                     {
      double yp                                   ;
      if ((i%10)==0) yp = T                  ; else
      if ((i% 5)==0) yp = T + 20             ; else
                     yp = T + 40                  ;
      p -> drawLine ( QLineF ( x , yp , x , V ) ) ;
    }                                             ;
    i++                                           ;
  } while (x<ScreenRect.width())                  ;
}

void N::VcfScreenSize::PaintImage(QPainter * p,QRectF Region)
{
  QGraphicsScene * s = scene()                            ;
  QList<QGraphicsView *> view = s->views()                ;
  if (view.count()<=0) return                             ;
  QGraphicsView * v = view [0]                            ;
  QRectF  R = Region                                      ;
  QSize   I = monitor.size()                              ;
  QPointF Z = v->mapToScene(QPoint(0,0))                  ;
  QPointF W = v->mapToScene(QPoint(I.width(),I.height())) ;
  W -= Z                                                  ;
  QRectF  P(R.left () + ((R.width ()-W.x())/2)            ,
            R.top  () + ((R.height()-W.y())/2)            ,
            W.x () , W.y ()                             ) ;
  p -> drawImage(P,monitor)                               ;
}

QBrush N::VcfScreenSize::brush(QRectF Region)
{
  QPointF start((Region.width()*1)/4,(Region.height()*1)/4) ;
  QPointF final((Region.width()*3)/4,(Region.height()*3)/4) ;
  QLinearGradient LG (start,final                         ) ;
  LG.setColorAt      (0,QColor(240,224,224)               ) ;
  LG.setColorAt      (1,QColor(224,240,224)               ) ;
  return QBrush      (LG                                  ) ;
}

void N::VcfScreenSize::Attribute (void)
{
  setAcceptHoverEvents (true                          ) ;
  setFlag              (ItemIsSelectable        ,true ) ;
  setFlag              (ItemIsMovable           ,false) ;
  setFlag              (ItemIsFocusable         ,true ) ;
  setFlag              (ItemClipsToShape        ,false) ;
  setFlag              (ItemClipsChildrenToShape,false) ;
  ///////////////////////////////////////////////////////
  for (int i=0;i<8;i++)                                 {
    Texts << new QGraphicsSimpleTextItem ( this )       ;
  }                                                     ;
  Texts [ 0 ] -> setText ( "1024"                     ) ;
  Texts [ 1 ] -> setText ( "768"                      ) ;
  Texts [ 0 ] -> setPos  ( QPointF( 60, 60)           ) ;
  Texts [ 1 ] -> setPos  ( QPointF(120, 60)           ) ;
  Texts [ 2 ] -> setPos  ( QPointF( 60, 60)           ) ;
  Texts [ 3 ] -> setPos  ( QPointF(120, 60)           ) ;
  Texts [ 4 ] -> setPos  ( QPointF( 60, 60)           ) ;
  Texts [ 5 ] -> setPos  ( QPointF(120, 60)           ) ;
  Texts [ 6 ] -> setPos  ( QPointF( 60, 60)           ) ;
  Texts [ 7 ] -> setPos  ( QPointF(120, 60)           ) ;
  ///////////////////////////////////////////////////////
  QTransform t                                          ;
  t . reset                   (     )                   ;
  t . rotate                  (  90 )                   ;
  Texts [ 1 ] -> setTransform ( t   )                   ;
  t . reset                   (     )                   ;
  t . rotate                  ( 270 )                   ;
  Texts [ 4 ] -> setTransform ( t   )                   ;
  Texts [ 5 ] -> setTransform ( t   )                   ;
  Texts [ 7 ] -> setTransform ( t   )                   ;
}

void N::VcfScreenSize::Park(QSize SIZE)
{
  QGraphicsScene * s = scene()                            ;
  QList<QGraphicsView *> view = s->views()                ;
  if (view.count()<=0) return                             ;
  QGraphicsView * v = view [0]                            ;
  QRect   S = qApp->desktop()->screenGeometry()           ;
  QRectF  R = ScreenRect                                  ;
  QSize   I = monitor.size()                              ;
  QPointF Z = v->mapToScene(QPoint(0,0))                  ;
  QPointF Y = v->mapToScene(QPoint(I.width(),I.height())) ;
  Y -= Z                                                  ;
  QRectF  P(R.left () + ((R.width ()-Y.x())/2)            ,
            R.top  () + ((R.height()-Y.y())/2)            ,
            Y.x () , Y.y ()                             ) ;
  QPointF X(ScreenRect.width(),ScreenRect.height())       ;
  QPoint  G = v->mapFromScene(X)                          ;
  int     WidthPixels  = S.width  ()                      ;
  int     WidthLength  = screen->widthLength (S.width ()) ;
  int     HeightPixels = S.height ()                      ;
  int     HeightLength = screen->heightLength(S.height()) ;
  screen->WidthPixels  = WidthPixels                      ;
  screen->WidthLength  = WidthLength                      ;
  screen->HeightPixels = HeightPixels                     ;
  screen->HeightLength = HeightLength                     ;
  int     ScreenWidth  = SIZE.width  ()                      ;
  int     ScreenWL     = screen->widthLength (SIZE.width ()) ;
  int     ScreenHeight = SIZE.height ()                      ;
  int     ScreenHL     = screen->heightLength(SIZE.height()) ;
  //////////////////////////////////////////////////////////////////////////
  int H = R.top  () + (R.height() / 8);
  int W = R.left () + (R.width () / 8);
  QPointF P11(R.center().x()-60,H                ) ;
  QPointF P12(R.left  ()    + 1,H                ) ;
  QPointF P13(R.center().x()+60,H                ) ;
  QPointF P14(R.right ()    - 1,H                ) ;
  QPointF P21(W                ,R.center().y()-60) ;
  QPointF P22(W                ,R.top   ()    + 1) ;
  QPointF P23(W                ,R.center().y()+60) ;
  QPointF P24(W                ,R.bottom()    - 1) ;
  //////////////////////////////////////////////////////////////////////////
  QPointF p(0,0.45);
  p = Options->position(p);
  QFont F = font ;
  F.setPixelSize(p.y()) ;
  for (int i=0;i<Texts.count();i++) Texts[i]->setFont(F) ;
  //////////////////////////////////////////////////////////////////////////
  Texts[0]->setText (tr("%1 pixels").arg(S.width ())) ;
  Texts[1]->setText (tr("%1 pixels").arg(S.height())) ;
  QRectF R1 = Texts[0]->boundingRect();
  QRectF R2 = Texts[1]->boundingRect();
  QPointF P1(P.center().x()-(R1.width ()/2),P.bottom()                   );
  QPointF P2(P.right ()    + R2.height()*2 ,P.center().y()-(R2.width()/2));
  Texts[0]->setPos(P1) ;
  Texts[1]->setPos(P2) ;
  //////////////////////////////////////////////////////////////////////////
  Texts[2]->setText(tr("%1 pixels").arg(ScreenWidth));
  Texts[3]->setText(tr("%1 mm"    ).arg(ScreenWL));
  QRectF T1 = Texts[2]->boundingRect();
  QRectF T2 = Texts[3]->boundingRect();
  Texts[2]->setPos(QPointF(P11.x()+((120-T1.width())/2),P11.y()-T1.height()-8));
  Texts[3]->setPos(QPointF(P11.x()+((120-T2.width())/2),P11.y()+8            ));
  //////////////////////////////////////////////////////////////////////////
  Texts[4]->setText(tr("%1 pixels").arg(ScreenHeight));
  Texts[5]->setText(tr("%1 mm").arg(ScreenHL));
  QRectF T3 = Texts[4]->boundingRect();
  QRectF T4 = Texts[5]->boundingRect();
  Texts[4]->setPos(QPointF(P23.x()-T3.height()-8,P23.y()-((120-T3.width())/2)));
  Texts[5]->setPos(QPointF(P23.x()            +8,P23.y()-((120-T4.width())/2)));
  //////////////////////////////////////////////////////////////////////////
  N::Screen XX ;
  XX.WidthPixels  = WidthPixels  ;
  XX.WidthLength  = WidthLength  ;
  XX.HeightPixels = HeightPixels ;
  XX.HeightLength = HeightLength ;
  Texts[6]->setText(tr("%1 mm").arg(XX.widthLength (S.width())));
  Texts[7]->setText(tr("%1 mm").arg(XX.heightLength(S.height())));
  QRectF R3 = Texts[0]->boundingRect();
  QRectF R4 = Texts[1]->boundingRect();
  QPointF P3(P.center().x()-(R3.width ()/2),P.top()-R3.height()          );
  QPointF P4(P.left ()     - R4.height()*2 ,P.center().y()+(R4.width()/2));
  Texts[6]->setPos(P3) ;
  Texts[7]->setPos(P4) ;
}
