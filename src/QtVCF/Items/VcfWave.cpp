#include <qtvcf.h>

N::VcfWave:: VcfWave   ( VcfHeadParaments  )
           : VcfCanvas ( parent , item , p )
{
  Configure ( ) ;
}

N::VcfWave::~VcfWave(void)
{
}

void N::VcfWave::Configure(void)
{
  Printable   = true                                    ;
  Scaling     = false                                   ;
  Editable    = true                                    ;
  DrawDot     = false                                   ;
  DrawLine    = true                                    ;
  PrivateMenu = true                                    ;
  HighQuality = false                                   ;
  ///////////////////////////////////////////////////////
  setFlag ( ItemIsMovable                 , true  )     ;
  setFlag ( ItemIsSelectable              , true  )     ;
  setFlag ( ItemIsFocusable               , true  )     ;
  setFlag ( ItemClipsToShape              , false )     ;
  setFlag ( ItemClipsChildrenToShape      , false )     ;
  setFlag ( ItemSendsGeometryChanges      , true  )     ;
  setFlag ( ItemSendsScenePositionChanges , true  )     ;
  ///////////////////////////////////////////////////////
  Painter . addMap   ( "Default" , 0                  ) ;
  Painter . addPen   ( 0 , QColor ( 192 , 192 , 192 ) ) ;
  Painter . addPen   ( 1 , QColor (   0 ,   0 , 255 ) ) ;
  Painter . addPen   ( 2 , QColor ( 255 ,   0 ,   0 ) ) ;
  Painter . addBrush ( 0 , QColor ( 252 , 252 , 252 ) ) ;
  Painter . addBrush ( 2 , QColor ( 255 ,   0 ,   0 ) ) ;
  ///////////////////////////////////////////////////////
  Total  = 0                                            ;
  Values = NULL                                         ;
  Shift  = 0                                            ;
  Factor = 1.0                                          ;
}

void N::VcfWave::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
  if (Menu(event->pos())) event->accept(); else
  QGraphicsItem :: contextMenuEvent ( event ) ;
}

QVariant N::VcfWave::itemChange(GraphicsItemChange change,const QVariant & value)
{
  switch (change)                                {
    case ItemPositionChange                      :
    case ItemPositionHasChanged                  :
      if (NotNull(Options))                      {
        QPointF p = QGraphicsItem::pos()         ;
        PaperPos = Options->Standard(p)          ;
      }                                          ;
    break                                        ;
    case ItemSelectedHasChanged                  :
      DeleteGadgets()                            ;
    break                                        ;
  }                                              ;
  return QGraphicsItem::itemChange(change,value) ;
}

void N::VcfWave::ResizeRect(QPointF P1,QPointF P2)
{
  double x1 = P1 . x ( )                   ;
  double x2 = P2 . x ( )                   ;
  double y1 = P1 . y ( )                   ;
  double y2 = P2 . y ( )                   ;
  double t  = 0                            ;
  if (x1>x2) { t = x1 ; x1 = x2 ; x2 = t ; }
  if (y1>y2) { t = y1 ; y1 = y2 ; y2 = t ; }
  ScreenRect . setLeft   ( x1 )            ;
  ScreenRect . setTop    ( y1 )            ;
  ScreenRect . setRight  ( x2 )            ;
  ScreenRect . setBottom ( y2 )            ;
  updateGeometry ( )                       ;
  update         ( )                       ;
}

void N::VcfWave::ResizeWidth(QPointF P1,QPointF P2)
{
  double x1 = P1.x()                       ;
  double x2 = P2.x()                       ;
  double t  = 0                            ;
  if (x1>x2) { t = x1 ; x1 = x2 ; x2 = t ; }
  ScreenRect . setLeft   ( x1 )            ;
  ScreenRect . setRight  ( x2 )            ;
  updateGeometry ( )                       ;
  update         ( )                       ;
}

void N::VcfWave::ResizeHeight(QPointF P1,QPointF P2)
{
  double y1 = P1.y()                       ;
  double y2 = P2.y()                       ;
  double t  = 0                            ;
  if (y1>y2) { t = y1 ; y1 = y2 ; y2 = t ; }
  ScreenRect . setTop    ( y1 )            ;
  ScreenRect . setBottom ( y2 )            ;
  updateGeometry ( )                       ;
  update         ( )                       ;
}

bool N::VcfWave::ResizeStart(QGraphicsSceneMouseEvent * event)
{
  if (!VcfRectangle::ResizeStart (event)) return false ;
  updateGeometry ( )                                   ;
  grabMouse      ( )                                   ;
  return true                                          ;
}

bool N::VcfWave::ResizeFinish(QGraphicsSceneMouseEvent * event)
{
  if (Markers[0]==0) return false            ;
  PaperRect  = Options->Standard(ScreenRect) ;
  Markers[0] = 0                             ;
  setCursor      ( Qt::ArrowCursor )         ;
  updateGeometry (                 )         ;
  update         (                 )         ;
  ungrabMouse    (                 )         ;
  return true                                ;
}

void N::VcfWave::Paint(QPainter * p,QRectF Region,bool clip,bool color)
{
  if (Painter . pathes . contains (0))                              {
    pushPainters  ( p                    )                          ;
    Painter . setPainter ( p , "Default" )                          ;
    p -> drawPath ( Painter . pathes [0] )                          ;
    popPainters   ( p                    )                          ;
  }                                                                 ;
  if (HighQuality)                                                  {
    p -> setRenderHint ( QPainter::Antialiasing            , true ) ;
    p -> setRenderHint ( QPainter::HighQualityAntialiasing , true ) ;
  }                                                                 ;
  if (DrawLine && Painter . pathes . contains (1))                  {
    pushPainters  ( p                    )                          ;
    p -> setPen   ( Painter . pens   [1] )                          ;
    p -> drawPath ( Painter . pathes [1] )                          ;
    popPainters   ( p                    )                          ;
  }                                                                 ;
  if (DrawDot && Points.count()>0)                                  {
    pushPainters  ( p                    )                          ;
    p -> setPen   ( Painter . pens   [2] )                          ;
    p -> setBrush ( Painter . brushes[2] )                          ;
    for (int i=0;i<Points.count();i++)                              {
      p->drawPoint(Points[i]             )                          ;
    }                                                               ;
    popPainters   ( p                    )                          ;
  }                                                                 ;
}

void N::VcfWave::Hovering(QPointF pos)
{
}

void N::VcfWave::updateGeometry(void)
{
  QPainterPath path             ;
  path . addRect ( ScreenRect ) ;
  Painter . pathes [ 0 ] = path ;
  updateWave ( )                ;
}

void N::VcfWave::setRange(QRectF paper)
{
  setPos  ( paper.topLeft()                             ) ;
  setRect ( QRectF ( 0,0,paper.width(),paper.height() ) ) ;
  updateGeometry ( )                                      ;
}

void N::VcfWave::setData(int total,double * values)
{
  Total  = total  ;
  Values = values ;
  updateWave ( )  ;
}

double N::VcfWave::Amplitude(int index)
{
  double v = Values[index] ;
  v -= Shift               ;
  return v * Factor        ;
}

void N::VcfWave::setFactor(double shift,double factor)
{
  Shift  = shift  ;
  Factor = factor ;
}

void N::VcfWave::updateWave(void)
{
  if (Total<=0      ) return          ;
  if (IsNull(Values)) return          ;
  /////////////////////////////////////
  double x = ScreenRect . left   ( )  ;
  double y = ScreenRect . top    ( )  ;
  double w = ScreenRect . width  ( )  ;
  double h = ScreenRect . height ( )  ;
  double d = w / (Total-1)            ;
  h /= 2                              ;
  y += h                              ;
  /////////////////////////////////////
  QPainterPath path                   ;
  QPointF p(x,y-(Amplitude(0)*h))     ;
  path.moveTo(p)                      ;
  Points .  clear ( )                 ;
  Points << p                         ;
  for (int i=1;i<Total;i++)           {
    x += d                            ;
    QPointF z(x,y-(Amplitude(i)*h))   ;
    path.lineTo(z)                    ;
    Points << z                       ;
  }                                   ;
  Painter . pathes [ 1 ] = path       ;
}

void N::VcfWave::Empty(double zero)
{
  if (Total<=0) return                       ;
  for (int i=0;i<Total;i++) Values[i] = zero ;
}

void N::VcfWave::Empty(int tail,double zero)
{
  if (Total<=0) return                                  ;
  int et = Total - tail                                 ;
  for (int i=0 ;i<et   ;i++) Values[i] = Values[i+tail] ;
  for (int i=et;i<Total;i++) Values[i] = zero           ;
}

bool N::VcfWave::Menu(QPointF pos)
{
  if (!PrivateMenu)                             {
    emit Menu ( this , pos )                    ;
    return true                                 ;
  }                                             ;
  VcfScopedMenu ( mm )                          ;
  Qt::PenStyle ps = Painter.pens[1].style()     ;
  QAction    * aa                               ;
  bool         sl = ( ps == Qt::SolidLine )     ;
  aa  = mm.add(101,tr( "Draw dots"  ) )         ;
  aa -> setCheckable ( true           )         ;
  aa -> setChecked   ( DrawDot        )         ;
  aa  = mm.add(102,tr( "Draw lines" ) )         ;
  aa -> setCheckable ( true           )         ;
  aa -> setChecked   ( DrawLine       )         ;
  aa  = mm.add(103,tr( "Solid line" ) )         ;
  aa -> setCheckable ( true           )         ;
  aa -> setChecked   ( sl             )         ;
  mm  . setFont      ( plan           )         ;
  aa  = mm.exec      (                )         ;
  if (IsNull(aa)) return true                   ;
  switch (mm[aa])                               {
    case 101                                    :
      DrawDot  = aa->isChecked()                ;
      updateGeometry ( )                        ;
      update         ( )                        ;
    break                                       ;
    case 102                                    :
      DrawLine = aa->isChecked()                ;
      updateGeometry ( )                        ;
      update         ( )                        ;
    break                                       ;
    case 103                                    :
      if (aa->isChecked())                      {
        Painter.pens[1].setStyle(Qt::SolidLine) ;
      } else                                    {
        Painter.pens[1].setStyle(Qt::DotLine  ) ;
      }                                         ;
      updateGeometry ( )                        ;
      update         ( )                        ;
    break                                       ;
  }                                             ;
  return true                                   ;
}
