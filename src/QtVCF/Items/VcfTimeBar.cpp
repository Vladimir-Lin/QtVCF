#include <qtvcf.h>

N::VcfTimeBar:: VcfTimeBar ( VcfHeadParaments  )
              : VcfCanvas  ( parent , item , p )
              , BarMode    ( Tracking          )
{
  Printable = false                                             ;
  Scaling   = false                                             ;
  Editable  = false                                             ;
  ///////////////////////////////////////////////////////////////
  setFlag(ItemIsMovable           ,false)                       ;
  setFlag(ItemIsSelectable        ,true )                       ;
  setFlag(ItemIsFocusable         ,true )                       ;
  setFlag(ItemClipsToShape        ,false)                       ;
  setFlag(ItemClipsChildrenToShape,false)                       ;
  ///////////////////////////////////////////////////////////////
  Painter . addMap   ( "Default" , 0                  )         ;
  Painter . addMap   ( "Range"   , 1                  )         ;
  Painter . addMap   ( "Mark"    , 2                  )         ;
  Painter . addPen   ( 0 , QColor ( 192 , 192 , 192 ) )         ;
  Painter . addPen   ( 1 , QColor (   0 , 192 , 192 ) )         ;
  Painter . addPen   ( 2 , QColor (   0 ,   0 , 224 ) )         ;
  Painter . addBrush ( 2 , QColor (   0 ,   0 , 224 ) )         ;
  ///////////////////////////////////////////////////////////////
  Painter . pens [ 1 ] . setStyle ( Qt::DashLine )              ;
  ///////////////////////////////////////////////////////////////
  QLinearGradient * linear                                      ;
  ///////////////////////////////////////////////////////////////
  Painter . gradients[0] = Gradient(QGradient::LinearGradient)  ;
  linear = Painter . gradients[0] .linear()                     ;
  linear->setColorAt(0.0,QColor(224,224,224))                   ;
  linear->setColorAt(0.5,QColor(252,252,252))                   ;
  linear->setColorAt(1.0,QColor(224,224,224))                   ;
  ///////////////////////////////////////////////////////////////
  Painter . gradients[1] = Gradient(QGradient::LinearGradient)  ;
  linear = Painter . gradients[1] .linear()                     ;
  linear->setColorAt(0.0,QColor(192,252,192))                   ;
  linear->setColorAt(0.5,QColor(240,252,240))                   ;
  linear->setColorAt(1.0,QColor(192,252,192))                   ;
  ///////////////////////////////////////////////////////////////
  nConnect ( &Timer,SIGNAL(timeout()),this,SLOT(Refresh()) )    ;
}

N::VcfTimeBar::~VcfTimeBar(void)
{
}

void N::VcfTimeBar::setGradient(int id)
{
  if (!Painter.gradients.contains(id)) return                        ;
  QLinearGradient * linear                                           ;
  linear = Painter . gradients[id] . linear()                        ;
  linear-> setStart     ( ScreenRect . topLeft   () )                ;
  linear-> setFinalStop ( ScreenRect . bottomLeft() )                ;
  Painter . brushes  [id] = Brush(*(Painter.gradients[id].gradient)) ;
}

void N::VcfTimeBar::Paint(QPainter * p,QRectF Region,bool clip,bool color)
{ Q_UNUSED    ( Region )                                     ;
  Q_UNUSED    ( clip   )                                     ;
  Q_UNUSED    ( color  )                                     ;
  if (!Painter.gradients.contains(0)) return                 ;
  if (!Painter.gradients.contains(1)) return                 ;
  ////////////////////////////////////////////////////////////
  setGradient ( 0      )                                     ;
  setGradient ( 1      )                                     ;
  ////////////////////////////////////////////////////////////
  QRectF R                                                   ;
  QDateTime T = Start                                        ;
  bool   range = false                                       ;
  if (Start<QDateTime::currentDateTime())                    {
    if (Final<QDateTime::currentDateTime())                  {
      R = ScreenRect                                         ;
    } else                                                   {
      int    dt = Start.secsTo(Final)                        ;
      int    ds = Start.secsTo(QDateTime::currentDateTime()) ;
      double dw = ScreenRect.width()                         ;
      dw *= ds ; dw /= dt                                    ;
      R = ScreenRect                                         ;
      R.setWidth(dw)                                         ;
    }                                                        ;
    range = true                                             ;
  }                                                          ;
  ////////////////////////////////////////////////////////////
  pushPainters         ( p                          )        ;
  Painter . drawRect   ( p , "Default" , ScreenRect )        ;
  if (range) Painter . drawRect ( p , "Range" , R   )        ;
  double y1 = ScreenRect . top    ( )                        ;
  double yd = ScreenRect . height ( )                        ;
  while (T<Final)                                            {
    QTime  t = T . time   ()                                 ;
    int    m = t . minute ()                                 ;
    double x = X(T)                                          ;
    if (m==0)                                                {
      if (t.hour()==0)                                       {
        QDateTime B                                          ;
        Painter . setPainter ( p , "Mark"           )        ;
        p->drawLine(QPointF(x,y1),QPointF(x,y1+yd))          ;
        B = T ; B = B.addSecs( 60) ; x = X(B)                ;
        p->drawLine(QPointF(x,y1),QPointF(x,y1+yd))          ;
        B = T ; B = B.addSecs(-60) ; x = X(B)                ;
        p->drawLine(QPointF(x,y1),QPointF(x,y1+yd))          ;
      } else                                                 {
        Painter . setPainter ( p , "Default"        )        ;
        p->drawLine(QPointF(x,y1),QPointF(x,y1+yd))          ;
      }                                                      ;
    } else
    if (m==30)                                               {
      Painter . setPainter ( p , "Default"          )        ;
      p->drawLine(QPointF(x,y1),QPointF(x,y1+(yd/2)))        ;
    } else
    if ((m%10)==0)                                           {
      Painter . setPainter ( p , "Default"          )        ;
      p->drawLine(QPointF(x,y1),QPointF(x,y1+(yd/4)))        ;
    } else
    if ((m%5)==0)                                            {
      Painter . setPainter ( p , "Default"          )        ;
      p->drawLine(QPointF(x,y1),QPointF(x,y1+(yd/8)))        ;
    }                                                        ;
    T = T.addSecs(60)                                        ;
  }                                                          ;
  popPainters          ( p                          )        ;
}

void N::VcfTimeBar::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  bool movable = IsMask(flags(),ItemIsMovable)  ;
  bool tracking = ( BarMode == Tracking )       ;
  if (!movable && tracking                     &&
       IsMask(event->buttons(),Qt::LeftButton)) {
    Positions[0] = event->pos()                 ;
    TimeLines[0] = Timeline(Positions[0].x())   ;
    TimeLines[2] = Start                        ;
    TimeLines[3] = Final                        ;
  } else                                        {
    scalePressEvent ( event )                   ;
    DeleteGadgets   (       )                   ;
  }                                             ;
}

void N::VcfTimeBar::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
  bool movable  = IsMask(flags(),ItemIsMovable) ;
  bool tracking = ( BarMode == Tracking )       ;
  if (!movable && tracking                     &&
       IsMask(event->buttons(),Qt::LeftButton)) {
    Positions[1] = event->pos()                 ;
    Moving()                                    ;
  } else                                        {
    scaleMoveEvent ( event )                    ;
  }                                             ;
}

void N::VcfTimeBar::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  bool movable  = IsMask(flags(),ItemIsMovable) ;
  bool tracking = ( BarMode == Tracking )       ;
  if (!movable && tracking)                     {
    Positions[1] = event->pos()                 ;
    Moving()                                    ;
    emit TimeUpdated ( )                        ;
  } else                                        {
    scaleReleaseEvent ( event )                 ;
  }                                             ;
}

void N::VcfTimeBar::setToday(void)
{
  QTime T(0,0,0,0)                     ;
  Start = QDateTime::currentDateTime() ;
  Start . setTime(T)                   ;
  Final = Start.addSecs(86400)         ;
  Timer . start(1000)                  ;
}

void N::VcfTimeBar::Refresh(void)
{
  update() ;
}

void N::VcfTimeBar::Hovering(QPointF pos)
{
  double dw = ScreenRect.width()                    ;
  double ds = pos.x() - ScreenRect.left()           ;
  int    dt = Start.secsTo(Final)                   ;
  QDateTime T                                       ;
  ds *= dt ; ds /= dw                               ;
  T = Start.addSecs((int)ds)                        ;
  QString M                                         ;
  M = T.toString(tr("yyyy/MM/dd AP hh:mm:ss dddd")) ;
  setToolTip ( M )                                  ;
  QToolTip::showText(QCursor::pos(),M)              ;
}

double N::VcfTimeBar::X(QDateTime T)
{
  int    dt = Start.secsTo(Final) ;
  int    ds = Start.secsTo(T    ) ;
  double dw = ScreenRect.width()  ;
  dw *= ds ; dw /= dt             ;
  return ScreenRect.left() + dw   ;
}

QDateTime N::VcfTimeBar::Timeline(double x)
{
  QDateTime T = Start               ;
  int    ds = Start.secsTo(Final)   ;
  double dw = ScreenRect.width()    ;
  double dx = x - ScreenRect.left() ;
  dx *= ds ; dx /= dw               ;
  T = T.addSecs(dx)                 ;
  return T                          ;
}

void N::VcfTimeBar::Moving(void)
{
  int       ds = Start.secsTo(Final)               ;
  double    dw = ScreenRect.width()                ;
  QPointF   D  = Positions[1] - Positions[0]       ;
  QDateTime T  = TimeLines[0]                      ;
  double    dt = D.x()                             ;
  dt *= ds ; dt /= dw                              ;
  Start = TimeLines[2].addSecs(-dt)                ;
  Final = TimeLines[3].addSecs(-dt)                ;
  update ( )                                       ;
}
