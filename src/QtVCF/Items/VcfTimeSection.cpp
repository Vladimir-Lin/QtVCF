#include <qtvcf.h>

N::VcfTimeSection:: VcfTimeSection ( VcfHeadParaments  )
                  : VcfCanvas      ( parent , item , p )
                  , Gantt          ( NULL              )
{
  Printable = false                                     ;
  Scaling   = false                                     ;
  Editable  = false                                     ;
  setFlag(ItemIsMovable           ,false)               ;
  setFlag(ItemIsSelectable        ,true )               ;
  setFlag(ItemIsFocusable         ,true )               ;
  setFlag(ItemClipsToShape        ,false)               ;
  setFlag(ItemClipsChildrenToShape,false)               ;
  Painter . addMap   ( "Default" , 0                  ) ;
  Painter . addPen   ( 0 , QColor ( 192 , 192 , 192 ) ) ;
  Painter . brushes . remove ( 0 )                      ;
  Painter . pens [ 0 ] . setStyle ( Qt::DotLine       ) ;
}

N::VcfTimeSection::~VcfTimeSection(void)
{
}

void N::VcfTimeSection::Paint(QPainter * p,QRectF Region,bool clip,bool color)
{
  QDateTime D                                       ;
  QDateTime E                                       ;
  QString   S                                       ;
  QTime     T(0,0,0,0)                              ;
  int       Alignment                               ;
  Alignment = Qt::AlignVCenter | Qt::AlignHCenter   ;
  if (Painter.fonts.contains(0))                    {
    Painter.fonts[0].setDPI(Options->DPI)           ;
  }                                                 ;
  pushPainters       ( p                          ) ;
  Painter . drawRect ( p , "Default" , ScreenRect ) ;
  if (Painter.fonts.contains(0))                    {
    p  -> setFont    ( Painter.fonts[0]           ) ;
  }                                                 ;
  D = Start                                         ;
  D . setTime        ( T                          ) ;
  E = D                                             ;
  E = E.addSecs      ( 86400                      ) ;
  while (D<Final)                                   {
    QRectF T  = ScreenRect                          ;
    double x1 = X(D)                                ;
    double x2 = X(E)                                ;
    T  . setLeft       (      x1                  ) ;
    T  . setWidth      ( x2 - x1                  ) ;
    S  = D.toString    ( tr("MM/dd dddd")         ) ;
    Painter . drawRect ( p , "Default" , T        ) ;
    p -> drawText      ( T , Alignment , S        ) ;
    D  = D . addSecs   ( 86400                    ) ;
    E  = D                                          ;
    E  = E.addSecs     ( 86400                    ) ;
  }                                                 ;
  popPainters        ( p                          ) ;
}

void N::VcfTimeSection::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  if (IsMask(event->buttons(),Qt::LeftButton))  {
    Positions[0] = event->pos()                 ;
    TimeLines[0] = Timeline(Positions[0].x())   ;
    TimeLines[2] = Start                        ;
    TimeLines[3] = Final                        ;
    GanttStart ( )                              ;
  } else                                        {
    scalePressEvent ( event )                   ;
    DeleteGadgets   (       )                   ;
  }                                             ;
}

void N::VcfTimeSection::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
  if (IsMask(event->buttons(),Qt::LeftButton)) {
    Positions[1] = event->pos()                ;
    GanttMoving ( )                            ;
  } else                                       {
    scaleMoveEvent ( event )                   ;
  }                                            ;
}

void N::VcfTimeSection::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if (isGantt())                {
    Positions[1] = event->pos() ;
    GanttFinished ( )           ;
  } else                        {
    scaleReleaseEvent ( event ) ;
  }                             ;
}

void N::VcfTimeSection::Hovering(QPointF pos)
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

double N::VcfTimeSection::X(QDateTime T)
{
  int    dt = Start.secsTo(Final) ;
  int    ds = Start.secsTo(T    ) ;
  double dw = ScreenRect.width()  ;
  dw *= ds ; dw /= dt             ;
  return ScreenRect.left() + dw   ;
}

QDateTime N::VcfTimeSection::Timeline(double x)
{
  QDateTime T = Start               ;
  int    ds = Start.secsTo(Final)   ;
  double dw = ScreenRect.width()    ;
  double dx = x - ScreenRect.left() ;
  dx *= ds ; dx /= dw               ;
  T = T.addSecs(dx)                 ;
  return T                          ;
}

bool N::VcfTimeSection::isGantt(void)
{
  return NotNull(Gantt) ;
}

void N::VcfTimeSection::GanttStart(void)
{
  if (NotNull(Gantt)) Gantt->deleteLater()        ;
  QPointF S = Options->Standard(Positions[0])     ;
  Gantt  = new VcfGantt(GraphicsView(),this,plan) ;
  Gantt -> Painter.fonts[0] = Painter.fonts[0]    ;
  Gantt -> Painter.fonts[1] = Painter.fonts[1]    ;
  Gantt -> Options   = Options                    ;
  Gantt -> Scaling   = false                      ;
  Gantt -> Start     = Timeline(Positions[0].x()) ;
  Gantt -> Final     = Timeline(Positions[0].x()) ;
  Gantt -> setZValue  ( 0.50f                   ) ;
  Gantt -> setOpacity ( 0.60f                   ) ;
  Gantt -> setPos     (QPointF(S.x(),0.125)     ) ;
  Gantt -> setRect    (QRectF(0,0,0.01,0.75)    ) ;
  Gantt -> show       (                         ) ;
}

void N::VcfTimeSection::GanttMoving(void)
{
  QPointF P1 = Positions[0]                        ;
  QPointF P2 = Positions[1]                        ;
  QPointF DP                                       ;
  QTime   TT                                       ;
  if (P1.x()>P2.x())                               {
    P1 = Positions[1]                              ;
    P2 = Positions[0]                              ;
  }                                                ;
  Gantt -> Start = Timeline(P1.x())                ;
  Gantt -> Final = Timeline(P2.x())                ;
  TT = Gantt -> Start . time ( )                   ;
  TT.setHMS(TT.hour(),TT.minute(),0)               ;
  Gantt -> Start . setTime (TT)                    ;
  TT = Gantt -> Final . time ( )                   ;
  TT.setHMS(TT.hour(),TT.minute(),0)               ;
  Gantt -> Final . setTime (TT)                    ;
  P1 . setX ( X ( Gantt->Start ) )                 ;
  P2 . setX ( X ( Gantt->Final ) )                 ;
  DP = P2 - P1                                     ;
  DP = Options->Standard(DP)                       ;
  P1 = Options->Standard(P1)                       ;
  Gantt -> setPos      ( QPointF(P1.x(),0.125)   ) ;
  Gantt -> setRect     ( QRectF(0,0,DP.x(),0.75) ) ;
  Gantt -> update      (                         ) ;
  Gantt -> GanttMoving (                         ) ;
}

void N::VcfTimeSection::GanttFinished(void)
{
  if (IsNull(Gantt)) return                ;
  QDateTime S = Gantt->Start               ;
  QDateTime F = Gantt->Final               ;
  if (NotNull(Gantt)) Gantt->deleteLater() ;
  Gantt = NULL                             ;
  emit Append ( S , F )                    ;
}
