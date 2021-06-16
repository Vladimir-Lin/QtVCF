#include <qtvcf.h>

#define EDITID  10001
#define TABLES(ID) plan->Tables[Tables::ID]

N::VcfGantt:: VcfGantt      ( VcfHeadParaments                    )
            : VcfCanvas     ( parent , item , p                   )
            , Uuid          ( 0                                   )
            , Task          ( 0                                   )
            , Owner         ( 0                                   )
            , TextAlignment ( Qt::AlignVCenter | Qt::AlignHCenter )
            , User          ( ""                                  )
            , Content       ( ""                                  )
            , Note          ( ""                                  )
            , EventType     ( (int)None                           )
            , Status        ( 0                                   )
            , Adjust        ( 0                                   )
{
  Configure ( ) ;
}

N::VcfGantt::~VcfGantt(void)
{
}

QRectF N::VcfGantt::boundingRect (void) const
{
  if (Painter.pathes.contains(0))           {
    return Painter.pathes[0].boundingRect() ;
  }                                         ;
  return ScreenRect                         ;
}

QPainterPath N::VcfGantt::shape(void) const
{
  if (Painter.pathes.contains(0)) {
    return Painter.pathes[0]      ;
  }                               ;
  QPainterPath path               ;
  path.addRect(ScreenRect)        ;
  return path                     ;
}

void N::VcfGantt::Configure(void)
{
  Printable = true                                              ;
  Scaling   = true                                              ;
  Editable  = true                                              ;
  Mode      = Board                                             ;
  setFlag(ItemIsMovable           ,false)                       ;
  setFlag(ItemIsSelectable        ,true )                       ;
  setFlag(ItemIsFocusable         ,true )                       ;
  setFlag(ItemClipsToShape        ,false)                       ;
  setFlag(ItemClipsChildrenToShape,false)                       ;
  Painter . addMap   ( "Default"  , 0                 )         ;
  Painter . addMap   ( "None"     , 2                 )         ;
  Painter . addMap   ( "Notify"   , 3                 )         ;
  Painter . addMap   ( "Expire"   , 4                 )         ;
  Painter . addMap   ( "Activate" , 5                 )         ;
  Painter . addMap   ( "Sleep"    , 6                 )         ;
  Painter . addPen   ( 0 , QColor ( 192 , 192 , 192 ) )         ;
  Painter . addPen   ( 1 , QColor (   0 ,   0 ,   0 ) )         ;
  Painter . addPen   ( 2 , QColor ( 224 , 192 , 192 ) )         ;
  Painter . addPen   ( 3 , QColor ( 192 , 192 , 224 ) )         ;
  Painter . addPen   ( 4 , QColor ( 224 , 224 , 192 ) )         ;
  Painter . addPen   ( 5 , QColor ( 192 , 224 , 224 ) )         ;
  Painter . addPen   ( 6 , QColor ( 224 , 192 , 224 ) )         ;
  Painter . addBrush ( 0 , QColor ( 240 , 240 , 240 ) )         ;
  QLinearGradient * linear                                      ;
  ///////////////////////////////////////////////////////////////
  Painter . gradients[2] = Gradient(QGradient::LinearGradient)  ;
  linear = Painter . gradients[2] .linear()                     ;
  linear->setColorAt(0.0,QColor(240,224,224))                   ;
  linear->setColorAt(0.5,QColor(255,240,240))                   ;
  linear->setColorAt(1.0,QColor(240,224,224))                   ;
  ///////////////////////////////////////////////////////////////
  Painter . gradients[3] = Gradient(QGradient::LinearGradient)  ;
  linear = Painter . gradients[3] .linear()                     ;
  linear->setColorAt(0.0,QColor(192,192,240))                   ;
  linear->setColorAt(0.5,QColor(240,240,255))                   ;
  linear->setColorAt(1.0,QColor(192,192,240))                   ;
  ///////////////////////////////////////////////////////////////
  Painter . gradients[4] = Gradient(QGradient::LinearGradient)  ;
  linear = Painter . gradients[4] .linear()                     ;
  linear->setColorAt(0.0,QColor(240,240,192))                   ;
  linear->setColorAt(0.5,QColor(255,255,240))                   ;
  linear->setColorAt(1.0,QColor(240,240,192))                   ;
  ///////////////////////////////////////////////////////////////
  Painter . gradients[5] = Gradient(QGradient::LinearGradient)  ;
  linear = Painter . gradients[5] .linear()                     ;
  linear->setColorAt(0.0,QColor(192,240,240))                   ;
  linear->setColorAt(0.5,QColor(240,255,255))                   ;
  linear->setColorAt(1.0,QColor(192,240,240))                   ;
  ///////////////////////////////////////////////////////////////
  Painter . gradients[6] = Gradient(QGradient::LinearGradient)  ;
  linear = Painter . gradients[6] .linear()                     ;
  linear->setColorAt(0.0,QColor(240,192,240))                   ;
  linear->setColorAt(0.5,QColor(255,240,255))                   ;
  linear->setColorAt(1.0,QColor(240,192,240))                   ;
  ///////////////////////////////////////////////////////////////
  Available . setWidth  ( 0 )                                   ;
  Available . setHeight ( 0 )                                   ;
  ///////////////////////////////////////////////////////////////
  TypeNames[None         ] = tr("None"         )                ;
  TypeNames[N::Notify    ] = tr("Notify"       )                ;
  TypeNames[Period       ] = tr("Period"       )                ;
  TypeNames[Record       ] = tr("Record"       )                ;
  TypeNames[Meeting      ] = tr("Meeting"      )                ;
  TypeNames[Automation   ] = tr("Automation"   )                ;
  TypeNames[Operation    ] = tr("Operation"    )                ;
  TypeNames[Sync         ] = tr("Sync"         )                ;
  TypeNames[Download     ] = tr("Download"     )                ;
  TypeNames[Upload       ] = tr("Upload"       )                ;
  TypeNames[Audio        ] = tr("Audio"        )                ;
  TypeNames[Video        ] = tr("Video"        )                ;
  TypeNames[TV           ] = tr("TV"           )                ;
  TypeNames[Programming  ] = tr("Programming"  )                ;
  TypeNames[Backup       ] = tr("Backup"       )                ;
  TypeNames[FileUpdate   ] = tr("FileUpdate"   )                ;
  TypeNames[Cooking      ] = tr("Cooking"      )                ;
  TypeNames[Sleep        ] = tr("Sleep"        )                ;
  TypeNames[Housework    ] = tr("Housework"    )                ;
  TypeNames[Shopping     ] = tr("Shopping"     )                ;
  TypeNames[Communication] = tr("Communication")                ;
  TypeNames[Finish       ] = tr("Finish"       )                ;
  TypeNames[Obsolete     ] = tr("Obsolete"     )                ;
  TypeNames[Others       ] = tr("Others"       )                ;
  ///////////////////////////////////////////////////////////////
  setDropFlag(DropText    ,true )                               ;
  setDropFlag(DropUrls    ,true )                               ;
  setDropFlag(DropImage   ,false)                               ;
  setDropFlag(DropHtml    ,true )                               ;
  setDropFlag(DropColor   ,true )                               ;
  setDropFlag(DropTag     ,false)                               ;
  setDropFlag(DropPicture ,false)                               ;
  setDropFlag(DropPeople  ,true )                               ;
  setDropFlag(DropAudio   ,true )                               ;
  setDropFlag(DropVideo   ,true )                               ;
  setDropFlag(DropAlbum   ,true )                               ;
  setDropFlag(DropGender  ,false)                               ;
  setDropFlag(DropDivision,false)                               ;
  setDropFlag(DropURIs    ,true )                               ;
  setDropFlag(DropBookmark,true )                               ;
  setDropFlag(DropFont    ,true )                               ;
  setDropFlag(DropPen     ,true )                               ;
  setDropFlag(DropBrush   ,true )                               ;
  setDropFlag(DropGradient,true )                               ;
  setDropFlag(DropShape   ,false)                               ;
  setDropFlag(DropMember  ,false)                               ;
  setDropFlag(DropSet     ,false)                               ;
}

void N::VcfGantt::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
  QGraphicsItem::hoverEnterEvent(event);
  if (!Scaling) return;
}

void N::VcfGantt::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
  QGraphicsItem::hoverLeaveEvent(event);
  if (!Scaling) return;
  setCursor(Qt::ArrowCursor);
}

void N::VcfGantt::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
  QGraphicsItem::hoverMoveEvent(event) ;
  Hovering (event->pos())              ;
  if (!Scaling) return                 ;
  int Corner = atCorner(event->pos())  ;
  switch (Corner)                      {
    case LeftSide                      :
    case RightSide                     :
      setCornerCursor(Corner)          ;
    break                              ;
    default                            :
      setCursor ( Qt::OpenHandCursor ) ;
    return                             ;
  }                                    ;
}

void N::VcfGantt::Hovering(QPointF pos)
{
}

void N::VcfGantt::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
  if (GanttMenu(event->pos())) event -> accept (       ) ;
  else QGraphicsItem::contextMenuEvent         ( event ) ;
}

void N::VcfGantt::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
  if (Editable)                                      {
    MountEditor     ( )                              ;
    event -> accept ( )                              ;
  } else QGraphicsItem::mouseDoubleClickEvent(event) ;
}

void N::VcfGantt::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  if (Scaling && IsMask(event->buttons(),Qt::LeftButton)) {
    if (ResizeStart ( event ) ) event -> accept () ; else {
      QGraphicsItem::mousePressEvent(event)               ;
    }                                                     ;
  } else                                                  {
    QGraphicsItem::mousePressEvent(event)                 ;
    DeleteGadgets()                                       ;
  }                                                       ;
}

void N::VcfGantt::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
  if (Scaling && IsMask(event->buttons(),Qt::LeftButton))  {
    if (ResizeMoving ( event ) ) event -> accept () ; else {
      QGraphicsItem::mouseMoveEvent(event)                 ;
    }                                                      ;
  } else if (Scaling)                                      {
    if (CursorMoving ( event ) ) event->accept () ; else   {
      QGraphicsItem::mouseMoveEvent(event)                 ;
    }                                                      ;
  } else QGraphicsItem::mouseMoveEvent(event)              ;
}

void N::VcfGantt::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if (Markers[0]==1) ResizeFinish(event) ;
  QGraphicsItem::mouseReleaseEvent(event);
}

bool N::VcfGantt::CursorMoving(QGraphicsSceneMouseEvent * event)
{
  int Corner = atCorner(event->pos())              ;
  switch (Corner)                                  {
    case NoSide                                    :
      setCursor(Qt::ArrowCursor    )               ;
    break                                          ;
    case LeftSide                                  :
      setCursor(Qt::SizeHorCursor  )               ;
    return true                                    ;
    case RightSide                                 :
      setCursor(Qt::SizeHorCursor  )               ;
    return true                                    ;
    default                                        :
      setCursor(Qt::OpenHandCursor )               ;
    break                                          ;
  }                                                ;
  return false                                     ;
}

bool N::VcfGantt::ResizeStart(QGraphicsSceneMouseEvent * event)
{
  int Corner = atCorner(event->pos())              ;
  switch (Corner)                                  {
    case NoSide                                    :
      setCursor(Qt::ArrowCursor    )               ;
      Markers    [0] = 0                           ;
      Markers    [1] = NoSide                      ;
      Markers    [9] = 0                           ;
    return false                                   ;
    case LeftSide                                  :
      setCursor(Qt::SizeHorCursor  )               ;
      Markers    [0] = 1                           ;
      Markers    [1] = Corner                      ;
      Markers    [9] = 0                           ;
      Points     [0] = event->pos()                ;
      Points     [2] = ScreenRect . topLeft  ()    ;
      Points     [3] = ScreenRect . topRight ()    ;
      Rectangles [0] = ScreenRect                  ;
      TimeLines  [0] = Start                       ;
      TimeLines  [1] = Final                       ;
    return true                                    ;
    case RightSide                                 :
      setCursor(Qt::SizeHorCursor  )               ;
      Markers    [0] = 1                           ;
      Markers    [1] = Corner                      ;
      Markers    [9] = 0                           ;
      Points     [0] = event->pos()                ;
      Points     [2] = ScreenRect . topRight ()    ;
      Points     [3] = ScreenRect . topLeft  ()    ;
      Rectangles [0] = ScreenRect                  ;
      TimeLines  [0] = Start                       ;
      TimeLines  [1] = Final                       ;
    return true                                    ;
    case Inside                                    :
      setCursor(Qt::OpenHandCursor )               ;
      Markers    [0] = 1                           ;
      Markers    [1] = Inside                      ;
      Markers    [9] = 0                           ;
      Points     [0] = event->pos()                ;
      Points     [2] = ScreenRect . topRight ()    ;
      Points     [3] = ScreenRect . topLeft  ()    ;
      Rectangles [0] = ScreenRect                  ;
      TimeLines  [0] = Start                       ;
      TimeLines  [1] = Final                       ;
    return true                                    ;
  }                                                ;
  return false                                     ;
}

bool N::VcfGantt::ResizeMoving(QGraphicsSceneMouseEvent * event)
{
  int     Corner = Markers [1]                       ;
  QPointF P1                                         ;
  QPointF P2                                         ;
  switch (Corner)                                    {
    case LeftSide                                    :
      Markers[9] = 1                                 ;
      Points [1] = event -> pos ()                   ;
      P1         = Points[2] + Points[1] - Points[0] ;
      P2         = Points[3]                         ;
      Start      = Timeline(Points[1].x())           ;
      ResizeWidth  ( P1 , P2 )                       ;
      GanttMoving  (         )                       ;
      emit Updated ( this    )                       ;
    return true                                      ;
    case RightSide                                   :
      Markers[9] = 1                                 ;
      Points [1] = event -> pos ()                   ;
      P1         = Points[2] + Points[1] - Points[0] ;
      P2         = Points[3]                         ;
      Final      = Timeline(Points[1].x())           ;
      ResizeWidth  ( P1 , P2 )                       ;
      GanttMoving  (         )                       ;
      emit Updated ( this    )                       ;
    return true                                      ;
    case Inside                                      :
      Markers[9] = 1                                 ;
      Points [1] = event -> pos ()                   ;
      DurationChanged ( )                            ;
      GanttMoving     ( )                            ;
      emit Updated ( this    )                       ;
    return true                                      ;
    case NoSide                                      :
    break                                            ;
  }                                                  ;
  return false                                       ;
}

bool N::VcfGantt::ResizeFinish(QGraphicsSceneMouseEvent * event)
{
  if (Markers[0]==0) return false            ;
  PaperRect  = Options->Standard(ScreenRect) ;
  Markers[0] = 0                             ;
  setCursor ( Qt::ArrowCursor )              ;
  update    (                 )              ;
  if (Markers[9]==1) emit Changed ( this )   ;
  Markers[9] = 0                             ;
  return true                                ;
}

void N::VcfGantt::Paint(QPainter * painter,QRectF Region,bool clip,bool color)
{
  switch (EventType)                                       {
    case None                                              :
      PaintNone        ( painter , Region , clip , color ) ;
    break                                                  ;
    case N::Notify                                         :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Period                                            :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Record                                            :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Meeting                                           :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Automation                                        :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Operation                                         :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Sync                                              :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Download                                          :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Upload                                            :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Audio                                             :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Video                                             :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case TV                                                :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Programming                                       :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Backup                                            :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case FileUpdate                                        :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Cooking                                           :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Sleep                                             :
      PaintSleep       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Housework                                         :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Shopping                                          :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Communication                                     :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Others                                            :
      PaintShape       ( painter , Region , clip , color ) ;
    break                                                  ;
    case Finish                                            :
    break                                                  ;
    case Obsolete                                          :
    break                                                  ;
    case Error                                             :
    break                                                  ;
    default                                                :
      VcfCanvas::Paint ( painter , Region , clip , color ) ;
      PaintText        ( painter , Region , clip , color ) ;
    break                                                  ;
  }                                                        ;
}

void N::VcfGantt::PaintShape(QPainter * p,QRectF Region,bool clip,bool color)
{
  if (IsNull(plan)) return                              ;
  QPainterPath path                                     ;
  QRectF    R = ContentRect ( )                         ;
  QDateTime C = QDateTime::currentDateTime()            ;
  ///////////////////////////////////////////////////////
  QLinearGradient * linear                              ;
  linear  = Painter . gradients[2] .linear()            ;
  linear -> setStart     ( ScreenRect . topLeft    () ) ;
  linear -> setFinalStop ( ScreenRect . bottomLeft () ) ;
  Painter.brushes[2] = Brush  ( *linear               ) ;
  ///////////////////////////////////////////////////////
  linear  = Painter . gradients[3] .linear()            ;
  linear -> setStart     ( ScreenRect . topLeft    () ) ;
  linear -> setFinalStop ( ScreenRect . bottomLeft () ) ;
  Painter.brushes[3] = Brush  ( *linear               ) ;
  ///////////////////////////////////////////////////////
  linear  = Painter . gradients[4] .linear()            ;
  linear -> setStart     ( ScreenRect . topLeft    () ) ;
  linear -> setFinalStop ( ScreenRect . bottomLeft () ) ;
  Painter.brushes[4] = Brush  ( *linear               ) ;
  ///////////////////////////////////////////////////////
  linear  = Painter . gradients[5] .linear()            ;
  linear -> setStart     ( ScreenRect . topLeft    () ) ;
  linear -> setFinalStop ( ScreenRect . bottomLeft () ) ;
  Painter.brushes[5] = Brush  ( *linear               ) ;
  ///////////////////////////////////////////////////////
  pushPainters       ( p                              ) ;
  ///////////////////////////////////////////////////////
  if (isSelected())                                     {
    Painter . drawRect ( p , "None"     , ScreenRect  ) ;
  } else
  if (C>Final)                                          {
    Painter . drawRect ( p , "Expire"   , ScreenRect  ) ;
  } else
  if (Start<C && C<Final)                               {
    Painter . drawRect ( p , "Activate" , ScreenRect  ) ;
  } else                                                {
    Painter . drawRect ( p , "Notify"   , ScreenRect  ) ;
  }                                                     ;
  path    . addRect    ( ScreenRect                   ) ;
  ///////////////////////////////////////////////////////
  if (Content.length()>0)                               {
    Painter.fonts[0].setDPI(Options->DPI)               ;
    QRectF A = Painter.boundingRect(0,Content)          ;
    if (A.width()<R.width())                            {
      p->setPen (Painter.pens [1])                      ;
      p->setFont(Painter.fonts[0])                      ;
      if (clip)                                         {
        p->drawText (R,TextAlignment,Content)           ;
      } else                                            {
        p->drawText (R,TextAlignment,Content)           ;
      }                                                 ;
    } else                                              {
      QRectF RT                                         ;
      QPointF D(0,0.125)                                ;
      double dw = (ScreenRect.height()-A.height()) / 2  ;
      D = Options -> position ( D )                     ;
      RT = ScreenRect                                   ;
      RT . setTop    (D.y () + ScreenRect.bottom ()   ) ;
      RT . setHeight (ScreenRect.height()             ) ;
      QRectF  LT = RT                                   ;
      QRectF  ET = RT                                   ;
      QRectF  MT = RT                                   ;
      QRectF  FT                                        ;
      QPointF MP = MT.center()                          ;
      FT . setTop    ( MT . top    () )                 ;
      FT . setBottom ( MT . bottom () )                 ;
      FT . setLeft   ( MP . x ( )-( A.width()/2)-dw  )  ;
      FT . setWidth  ( A . width ( ) + (dw * 2) )       ;
//      if (Available.contains(FT))                       {
        QPolygonF PS                                    ;
        PS << FT.topRight() ;
        PS << FT.bottomRight() ;
        PS << FT.bottomLeft() ;
        PS << FT.topLeft();
        PS << QPointF(MP.x()-D.y(),FT.top());
        PS << QPointF(MP.x()      ,FT.top()-(D.y()*2)) ;
        PS << QPointF(MP.x()+D.y(),FT.top());
        p->setPen      (Painter.pens   [3])             ;
        p->setBrush    (Painter.brushes[0])             ;
//        p->setBrush    (Painter.Brushes[3])             ;
        p->drawPolygon (PS)                             ;
        p->setFont     (Painter.fonts[0])               ;
        p->setPen      (Painter.pens   [1])             ;
        p->drawText    (FT,TextAlignment,Content)       ;
        path.addPolygon(PS)                             ;
//      }                                                 ;
    }                                                   ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  popPainters        ( p                              ) ;
  Painter.pathes[0] = path                              ;
}

void N::VcfGantt::PaintSleep(QPainter * p,QRectF Region,bool clip,bool color)
{
  if (IsNull(plan)) return                              ;
  QPainterPath path                                     ;
  QRectF    R = ContentRect ( )                         ;
  QDateTime C = QDateTime::currentDateTime()            ;
  ///////////////////////////////////////////////////////
  QLinearGradient * linear                              ;
  linear  = Painter . gradients[2] .linear()            ;
  linear -> setStart     ( ScreenRect . topLeft    () ) ;
  linear -> setFinalStop ( ScreenRect . bottomLeft () ) ;
  Painter.brushes[2] = Brush  ( *linear               ) ;
  ///////////////////////////////////////////////////////
  linear  = Painter . gradients[3] .linear()            ;
  linear -> setStart     ( ScreenRect . topLeft    () ) ;
  linear -> setFinalStop ( ScreenRect . bottomLeft () ) ;
  Painter.brushes[3] = Brush  ( *linear               ) ;
  ///////////////////////////////////////////////////////
  linear  = Painter . gradients[4] .linear()            ;
  linear -> setStart     ( ScreenRect . topLeft    () ) ;
  linear -> setFinalStop ( ScreenRect . bottomLeft () ) ;
  Painter.brushes[4] = Brush  ( *linear               ) ;
  ///////////////////////////////////////////////////////
  linear  = Painter . gradients[5] .linear()            ;
  linear -> setStart     ( ScreenRect . topLeft    () ) ;
  linear -> setFinalStop ( ScreenRect . bottomLeft () ) ;
  Painter.brushes[5] = Brush  ( *linear               ) ;
  ///////////////////////////////////////////////////////
  linear  = Painter . gradients[6] .linear()            ;
  linear -> setStart     ( ScreenRect . topLeft    () ) ;
  linear -> setFinalStop ( ScreenRect . bottomLeft () ) ;
  Painter.brushes[6] = Brush  ( *linear               ) ;
  ///////////////////////////////////////////////////////
  pushPainters       ( p                              ) ;
  ///////////////////////////////////////////////////////
  if (isSelected())                                     {
    Painter . drawRect ( p , "None"     , ScreenRect  ) ;
  } else
  if (C>Final)                                          {
    Painter . drawRect ( p , "Expire"   , ScreenRect  ) ;
  } else
  if (Start<C && C<Final)                               {
    Painter . drawRect ( p , "Sleep"    , ScreenRect  ) ;
  } else                                                {
    Painter . drawRect ( p , "Notify"   , ScreenRect  ) ;
  }                                                     ;
  QImage Icon (":/images/sleep.png")                    ;
  QRectF IconRect                                       ;
  IconRect.setTop   ( ScreenRect.top   ()+2 )           ;
  IconRect.setBottom( ScreenRect.bottom()-2 )           ;
  IconRect.setLeft  ( ScreenRect.left  ()+2 )           ;
  IconRect.setWidth ( IconRect  .height()   )           ;
  p -> drawImage    ( IconRect , Icon       )           ;
  path    . addRect    ( ScreenRect                   ) ;
  ///////////////////////////////////////////////////////
  if (Content.length()>0)                               {
    Painter.fonts[0].setDPI(Options->DPI)               ;
    QRectF A = Painter.boundingRect(0,Content)          ;
    if (A.width()<R.width())                            {
      p->setPen (Painter.pens [1])                      ;
      p->setFont(Painter.fonts[0])                      ;
      if (clip)                                         {
        p->drawText (R,TextAlignment,Content)           ;
      } else                                            {
        p->drawText (R,TextAlignment,Content)           ;
      }                                                 ;
    } else                                              {
      QRectF RT                                         ;
      QPointF D(0,0.125)                                ;
      double dw = (ScreenRect.height()-A.height()) / 2  ;
      D = Options -> position ( D )                     ;
      RT = ScreenRect                                   ;
      RT . setTop    (D.y () + ScreenRect.bottom ()   ) ;
      RT . setHeight (ScreenRect.height()             ) ;
      QRectF  LT = RT                                   ;
      QRectF  ET = RT                                   ;
      QRectF  MT = RT                                   ;
      QRectF  FT                                        ;
      QPointF MP = MT.center()                          ;
      FT . setTop    ( MT . top    () )                 ;
      FT . setBottom ( MT . bottom () )                 ;
      FT . setLeft   ( MP . x ( )-( A.width()/2)-dw  )  ;
      FT . setWidth  ( A . width ( ) + (dw * 2) )       ;
//      if (Available.contains(FT))                       {
        QPolygonF PS                                    ;
        PS << FT.topRight() ;
        PS << FT.bottomRight() ;
        PS << FT.bottomLeft() ;
        PS << FT.topLeft();
        PS << QPointF(MP.x()-D.y(),FT.top());
        PS << QPointF(MP.x()      ,FT.top()-(D.y()*2)) ;
        PS << QPointF(MP.x()+D.y(),FT.top());
        p->setPen      (Painter.pens   [3])             ;
        p->setBrush    (Painter.brushes[0])             ;
//        p->setBrush    (Painter.Brushes[3])             ;
        p->drawPolygon (PS)                             ;
        p->setFont     (Painter.fonts[0])               ;
        p->setPen      (Painter.pens   [1])             ;
        p->drawText    (FT,TextAlignment,Content)       ;
        path.addPolygon(PS)                             ;
//      }                                                 ;
    }                                                   ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  popPainters        ( p                              ) ;
  Painter.pathes[0] = path                              ;
}

void N::VcfGantt::PaintText(QPainter * p,QRectF region,bool clip,bool color)
{
  if (IsNull(plan)) return                ;
  QRectF R = ContentRect ( )              ;
  pushPainters  (p                   )    ;
  Painter.fonts[0].setDPI(Options->DPI)   ;
  p->setFont(Painter.fonts[0])            ;
  if (clip)                               {
    p->drawText (R,TextAlignment,Content) ;
  } else                                  {
    p->drawText (R,TextAlignment,Content) ;
  }                                       ;
  popPainters   (p                      ) ;
}

void N::VcfGantt::PaintNone(QPainter * p,QRectF Region,bool clip,bool color)
{
  QLinearGradient * linear                              ;
  linear  = Painter . gradients[2] .linear()            ;
  linear -> setStart     ( ScreenRect . topLeft    () ) ;
  linear -> setFinalStop ( ScreenRect . bottomLeft () ) ;
  Painter.brushes[2] = Brush  ( *linear               ) ;
  pushPainters       ( p                              ) ;
  Painter . drawRect ( p , "None" , ScreenRect        ) ;
  popPainters        ( p                              ) ;
}

QDateTime N::VcfGantt::Timeline(double x)
{
  QDateTime Z                                    ;
  double    dw =     Available . width ( )       ;
  double    dx = x - Available . left  ( )       ;
  int       ds = TabletStart.secsTo(TabletFinal) ;
  dx *= ds ; dx /= dw                            ;
  Z = TabletStart                                ;
  Z = Z.addSecs(dx)                              ;
  QTime T = Z.time()                             ;
  T . setHMS  ( T.hour() , T.minute() , 0      ) ;
  Z . setTime ( T                              ) ;
  return    Z                                    ;
}

void N::VcfGantt::setText(QString text)
{
  Content = text            ;
  QGraphicsItem::update ( ) ;
}

QRectF N::VcfGantt::ContentRect(void)
{
  double ls = Options->LineSpace                      ;
  QPointF G(ls,ls)                                    ;
  G = Options->position(G)                            ;
  return QRectF(ScreenRect . left   ( ) +  G.x()      ,
                ScreenRect . top    ( ) +  G.y()      ,
                ScreenRect . width  ( ) - (G.x()*2)   ,
                ScreenRect . height ( ) - (G.y()*2) ) ;
}

QRectF N::VcfGantt::EditorRect(void)
{
  double ls = Options->LineSpace                          ;
  QPointF G(ls,ls)                                        ;
  G = Options->position(G)                                ;
  QRectF X = QRectF(ScreenRect . left   ( ) +  G.x()      ,
                    ScreenRect . top    ( ) +  G.y()      ,
                    ScreenRect . width  ( ) - (G.x()*2)   ,
                    ScreenRect . height ( ) - (G.y()*2) ) ;
  QRectF XM = Options->Standard(X)                        ;
  if (XM.width()<2.5)                                     {
    G.setX(2.5)                                           ;
    G.setY(2.5)                                           ;
    G = Options->position(G)                              ;
    X . setWidth ( G . x ( ) )                            ;
  }                                                       ;
  return X                                                ;
}

QSizeF N::VcfGantt::FitSize(void)
{
  Painter.fonts[0].setDPI(Options->DPI) ;
  QFontMetricsF FMF(Painter.fonts[0])   ;
  QRectF  R = FMF.boundingRect(Content) ;
  QPointF S (R.width(),R.height())      ;
  S = Options -> Standard ( S )         ;
  return QSizeF ( S . x () , S . y () ) ;
}

void N::VcfGantt::MountEditor(void)
{
  QLineEdit * LE = NewLineEdit ( EDITID )    ;
  QRectF      ER = EditorRect  (        )    ;
  Proxys[EDITID]->setGeometry  ( ER     )    ;
  Proxys[EDITID]->setOpacity   (1.00    )    ;
  Proxys[EDITID]->setZValue    (1.00    )    ;
  Painter.fonts[0].setDPI(Options->DPI  )    ;
  LE -> setFont ( Painter.fonts[0]      )    ;
  LE -> setText ( Content               )    ;
  connect(LE  ,SIGNAL(editingFinished() )    ,
          this,SLOT  (NameFinished   () )  ) ;
  Alert ( Click )                            ;
  LE -> setFocus ( Qt::TabFocusReason      ) ;
}

void N::VcfGantt::NameFinished(void)
{
  QLineEdit * LE = qobject_cast<QLineEdit *>(Widgets[EDITID]) ;
  if (NotNull(LE))                                            {
    Content = LE -> text ( )                                  ;
  }                                                           ;
  DeleteGadgets ( )                                           ;
  update        ( )                                           ;
  emit Changed ( this )                                       ;
}

void N::VcfGantt::GanttUpdated(void)
{
  QString Q                                                            ;
  if (Uuid<=0)                                                         {
    Q  = tr("New event")                                               ;
    Q += "\n"                                                          ;
    Q += tr("Type:")                                                   ;
    Q += TypeNames[EventType]                                          ;
  } else                                                               {
    QTime   DT (0,0,0,0)                                               ;
    int     ds   = Start . secsTo  ( Final )                           ;
    int     Days = ds / 86400                                          ;
    QString FS   = Start . toString(tr("yyyy/MM/dd AP hh:mm:ss dddd")) ;
    QString TS   = Final . toString(tr("yyyy/MM/dd AP hh:mm:ss dddd")) ;
    DT = DT.addSecs(ds%86400)                                          ;
    Q  = Content                                                       ;
    Q += "\n"                                                          ;
    Q += tr("Type:")                                                   ;
    Q += TypeNames[EventType]                                          ;
    Q += "\n"                                                          ;
    Q += tr("Start:%1\nFinal:%2\n").arg(FS).arg(TS)                    ;
    Q += tr("Duration:")                                               ;
    if (Days==1) Q += tr("%1 day " ).arg(Days)                         ;
    if (Days> 1) Q += tr("%1 days ").arg(Days)                         ;
    Q += DT.toString(tr("hh:mm:ss"))                                   ;
    if (Note.length()>0)                                               {
      Q += "\n"                                                        ;
      Q += Note                                                        ;
    }                                                                  ;
  }                                                                    ;
  setToolTip(Q)                                                        ;
}

void N::VcfGantt::GanttMoving(void)
{
  QTime   DT (0,0,0,0)                                               ;
  int     ds   = Start . secsTo  ( Final                           ) ;
  int     Days = ds / 86400                                          ;
  QString FS   = Start . toString(tr("yyyy/MM/dd AP hh:mm:ss dddd")) ;
  QString TS   = Final . toString(tr("yyyy/MM/dd AP hh:mm:ss dddd")) ;
  QString M    = tr("Start:%1\nFinal:%2\n").arg(FS).arg(TS)          ;
  DT = DT.addSecs(ds%86400)                                          ;
  M += tr("Duration:")                                               ;
  if (Days==1) M += tr("%1 day " ).arg(Days)                         ;
  if (Days> 1) M += tr("%1 days ").arg(Days)                         ;
  M += DT.toString(tr("hh:mm:ss"))                                   ;
  QToolTip::showText(QCursor::pos(),M)                               ;
}

void N::VcfGantt::DurationChanged(void)
{
  QPointF P1                                    ;
  QPointF P2                                    ;
  double  dx = Points[1].x() - Points[0].x()    ;
  double  dw = Available . width ( )            ;
  int     ds = TabletStart.secsTo(TabletFinal)  ;
  int     dt = ds                               ;
  QTime   T                                     ;
  dx *= ds ; dx /= dw                           ;
  Start = TimeLines[0]                          ;
  Final = TimeLines[1]                          ;
  Start = Start . addSecs ( dx )                ;
  Final = Final . addSecs ( dx )                ;
  T     = Start . time    (    )                ;
  T     . setHMS  ( T.hour() , T.minute() , 0 ) ;
  Start . setTime ( T                         ) ;
  T     = Final . time    (    )                ;
  T     . setHMS  ( T.hour() , T.minute() , 0 ) ;
  Final . setTime ( T                         ) ;
  QDateTime S = Start                           ;
  QDateTime F = Final                           ;
  if (S<TabletStart) S = TabletStart            ;
  if (F>TabletFinal) F = TabletFinal            ;
  ds  = TabletStart.secsTo(S)                   ;
  dx  = ds ; dx *= dw ; dx /= dt                ;
  dx += Available.left()                        ;
  P1  . setX ( dx )                             ;
  ds  = TabletStart.secsTo(F)                   ;
  dx  = ds ; dx *= dw ; dx /= dt                ;
  dx += Available.left()                        ;
  P2  . setX ( dx )                             ;
  ResizeWidth ( P1 , P2 )                       ;
}

bool N::VcfGantt::dropText(QWidget * source,QPointF pos,const QString & text)
{
  return false ;
}

bool N::VcfGantt::dropUrls(QWidget * source,QPointF pos,const QList<QUrl> & urls)
{
  return false ;
}

bool N::VcfGantt::dropHtml(QWidget * source,QPointF pos,const QString & html)
{
  return false ;
}

bool N::VcfGantt::dropColor(QWidget * source,QPointF pos,const QColor & color)
{
  return false ;
}

bool N::VcfGantt::dropPeople(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return false ;
}

bool N::VcfGantt::dropVideos(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return false ;
}

bool N::VcfGantt::dropAlbums(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return false ;
}

bool N::VcfGantt::dropURIs(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return false ;
}

bool N::VcfGantt::dropBookmarks(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return false ;
}

bool N::VcfGantt::dropFont(QWidget * source,QPointF pos,const SUID font)
{
  return false ;
}

bool N::VcfGantt::dropPen(QWidget * source,QPointF pos,const SUID pen)
{
  return false ;
}

bool N::VcfGantt::dropBrush(QWidget * source,QPointF pos,const SUID brush)
{
  return false ;
}

bool N::VcfGantt::dropGradient(QWidget * source,QPointF pos,const SUID gradient)
{
  return false ;
}

bool N::VcfGantt::GanttMenu(QPointF pos)
{
  VcfScopedMenu ( mm )                                          ;
  QDateTime Now = QDateTime::currentDateTime()                  ;
  QMenu      * ma = NULL                                        ;
  QMenu      * mt = NULL                                        ;
  QMenu      * mx = NULL                                        ;
  QMenu      * me = NULL                                        ;
  QAction    * aa = NULL                                        ;
  ma = mm.addMenu(tr("Adjustment"   ))                          ;
  mt = mm.addMenu(tr("Type"         ))                          ;
  mx = mm.addMenu(tr("Attachments"  ))                          ;
  ///////////////////////////////////////////////////////////////
  switch (EventType)                                            {
    case Audio                                                  :
    case Video                                                  :
    case Meeting                                                :
    case Shopping                                               :
    case Communication                                          :
    case Programming                                            :
    case Backup                                                 :
      mm.add(ma,101,tr("Edit"))                                 ;
    break                                                       ;
    case Period                                                 :
    case Record                                                 :
    case Automation                                             :
    case Operation                                              :
    case Sync                                                   :
    case Download                                               :
    case Upload                                                 :
    case TV                                                     :
    case FileUpdate                                             :
    case N::Notify                                              :
    case Cooking                                                :
    case Sleep                                                  :
    case Housework                                              :
    case Others                                                 :
    break                                                       ;
    default                                                     :
    break                                                       ;
  }                                                             ;
  mm.add(ma,102,tr("Set start time" ))                          ;
  mm.add(ma,103,tr("Set finish time"))                          ;
  mm.add(ma,104,tr("Delete"         ))                          ;
  if (Now<Start) mm.add(ma,105,tr("Job start now"  ))           ;
  if (Now>Start) mm.add(ma,106,tr("Job finished"))              ;
  #define ETAC(ID)                                              \
    aa->setCheckable(true) ;                                    \
    if (ID==EventType) aa->setChecked(true)
  aa = mm.add(mt,801,tr("Notify"       )) ; ETAC(N::Notify    ) ;
  aa = mm.add(mt,802,tr("Period"       )) ; ETAC(Period       ) ;
  aa = mm.add(mt,803,tr("Record"       )) ; ETAC(Record       ) ;
  aa = mm.add(mt,804,tr("Meeting"      )) ; ETAC(Meeting      ) ;
  aa = mm.add(mt,805,tr("Automation"   )) ; ETAC(Automation   ) ;
  aa = mm.add(mt,806,tr("Operation"    )) ; ETAC(Operation    ) ;
  aa = mm.add(mt,807,tr("Sync"         )) ; ETAC(Sync         ) ;
  aa = mm.add(mt,808,tr("Download"     )) ; ETAC(Download     ) ;
  aa = mm.add(mt,809,tr("Upload"       )) ; ETAC(Upload       ) ;
  aa = mm.add(mt,810,tr("Audio"        )) ; ETAC(Audio        ) ;
  aa = mm.add(mt,811,tr("Video"        )) ; ETAC(Video        ) ;
  aa = mm.add(mt,812,tr("TV"           )) ; ETAC(TV           ) ;
  aa = mm.add(mt,813,tr("Programming"  )) ; ETAC(Programming  ) ;
  aa = mm.add(mt,814,tr("Backup"       )) ; ETAC(Backup       ) ;
  aa = mm.add(mt,815,tr("FileUpdate"   )) ; ETAC(FileUpdate   ) ;
  aa = mm.add(mt,816,tr("Cooking"      )) ; ETAC(Cooking      ) ;
  aa = mm.add(mt,817,tr("Sleep"        )) ; ETAC(Sleep        ) ;
  aa = mm.add(mt,818,tr("Housework"    )) ; ETAC(Housework    ) ;
  aa = mm.add(mt,819,tr("Shopping"     )) ; ETAC(Shopping     ) ;
  aa = mm.add(mt,820,tr("Communication")) ; ETAC(Communication) ;
  aa = mm.add(mt,821,tr("Others"       )) ; ETAC(Others       ) ;
  #undef  ETAC
  mm.add(mx,901,tr("Owner"               )                    ) ;
  mm.add(mx,902,tr("Task"                )                    ) ;
  mm.add(mx,903,tr("Note"                )                    ) ;
  mm.add(mx,904,tr("Set start trigger"   )                    ) ;
  mm.add(mx,905,tr("Set finish trigger"  )                    ) ;
  mm.add(mx,906,tr("Modification history")                    ) ;
  if (EventType==Automation)                                    {
    me = mm.addMenu(tr("Automation"  ))                         ;
    mm.add(me,701,tr("Clear execution"))                        ;
    mm.add(me,702,tr("Set execution"  ))                        ;
  }                                                             ;
  mm . setFont ( plan           )                               ;
  aa = mm.exec ( QCursor::pos() )                               ;
  if (IsNull(aa)) return true                                   ;
  switch (mm[aa])                                               {
    case 101                                                    :
    case 102                                                    :
    case 103                                                    :
    case 104                                                    :
    case 105                                                    :
    case 106                                                    :
    case 901                                                    :
    case 902                                                    :
    case 903                                                    :
    case 904                                                    :
    case 905                                                    :
    case 906                                                    :
      emit Edit ( this , mm[aa] )                               ;
    break                                                       ;
    case 701                                                    :
      ClearExecution ( )                                        ;
    break                                                       ;
    case 702                                                    :
      MountExecution ( )                                        ;
    break                                                       ;
    case 801                                                    :
      EventType = N::Notify                                     ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 802                                                    :
      EventType = Period                                        ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 803                                                    :
      EventType = Record                                        ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 804                                                    :
      EventType = Meeting                                       ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 805                                                    :
      EventType = Automation                                    ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 806                                                    :
      EventType = Operation                                     ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 807                                                    :
      EventType = Sync                                          ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 808                                                    :
      EventType = Download                                      ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 809                                                    :
      EventType = Upload                                        ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 810                                                    :
      EventType = Audio                                         ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 811                                                    :
      EventType = Video                                         ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 812                                                    :
      EventType = TV                                            ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 813                                                    :
      EventType = Programming                                   ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 814                                                    :
      EventType = Backup                                        ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 815                                                    :
      EventType = FileUpdate                                    ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 816                                                    :
      EventType = Cooking                                       ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 817                                                    :
      EventType = Sleep                                         ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 818                                                    :
      EventType = Housework                                     ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 819                                                    :
      EventType = Shopping                                      ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 820                                                    :
      EventType = Communication                                 ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
    case 821                                                    :
      EventType = Others                                        ;
      DropChanged  (      )                                     ;
      emit Changed ( this )                                     ;
    break                                                       ;
  }                                                             ;
  return true                                                   ;
}

void N::VcfGantt::ClearExecution(void)
{
  EnterSQL         ( SC , plan->sql          ) ;
    QString Q                                  ;
    Q = SC.sql.DeleteFrom(TABLES(Groups)       ,
      QString("where first = %1 and t1 = %2 and t2 = %3 and relation = %4")
      .arg(Uuid                                )
      .arg(Types::Schedule           )
      .arg(Types::Execution          )
      .arg(Groups::Subordination     )
    )                                          ;
    SC.Query(Q)                                ;
  LeaveSQL         ( SC , plan->sql          ) ;
  Alert ( Done )                               ;
}

void N::VcfGantt::MountExecution(void)
{
  QPainterPath path = shape()                                         ;
  QRectF       bret = path . boundingRect (  )                        ;
  QRectF       vret ( 0.0 , 0.0 , 4.0 , 0.72 )                        ;
  vret = Options -> Region ( vret )                                   ;
  QRectF cbrt(bret.left(),bret.bottom(),vret.width(),vret.height())   ;
  QComboBox * combo = NewComboBox   ( EDITID )                        ;
  if (IsNull(combo)) return                                           ;
  EnterSQL         ( SC , plan->sql          )                        ;
    QString Q                                                         ;
    QString name                                                      ;
    SUID    uuid                                                      ;
    SUID    euid  =  0                                                ;
    int     index = -1                                                ;
    int     cnt   =  0                                                ;
    UUIDs   Uuids = SC.Uuids(TABLES(Executions)                       ,
                    "uuid","order by id asc"                        ) ;
    Q = SC.sql.SelectFrom("second",TABLES(Groups)                     ,
        QString("where first = %1 and t1 = %2 and t2 = %3 and relation = %4")
        .arg(Uuid                                                     )
        .arg(Types::Schedule                                )
        .arg(Types::Execution                               )
        .arg(Groups::Subordination                          )
    )                                                                 ;
    if (SC.Fetch(Q)) euid = SC.Uuid(0)                                ;
    foreach (uuid,Uuids)                                              {
      name   = SC.getName(TABLES(Names),"uuid",vLanguageId,uuid)      ;
      combo -> addItem ( name , uuid )                                ;
      if (euid==uuid) index = cnt                                     ;
      cnt ++                                                          ;
    }                                                                 ;
    if (index>=0) combo->setCurrentIndex(index)                       ;
  LeaveSQL         ( SC , plan->sql          )                        ;
  Proxys [ EDITID ] -> setGeometry  ( cbrt   )                        ;
  Proxys [ EDITID ] -> setOpacity   ( 1.00   )                        ;
  Proxys [ EDITID ] -> setZValue    ( 1.00   )                        ;
  Painter . fonts[0] . setDPI ( Options->DPI     )                    ;
  combo  -> setFont           ( Painter.fonts[0] )                    ;
  connect(combo,SIGNAL(activated       (int) )                        ,
          this ,SLOT  (ExecutionChanged(int) )                      ) ;
  combo  -> showPopup ( )                                             ;
}

void N::VcfGantt::ExecutionChanged(int index)
{
  QComboBox * combo = Casting(QComboBox,Widgets[EDITID])                  ;
  if (IsNull(combo)) return                                               ;
  SUID euid = N::GetUuid(combo)                                           ;
  EnterSQL         ( SC , plan->sql          )                            ;
    QString Q                                                             ;
    Q = SC.sql.DeleteFrom(TABLES(Groups)                                  ,
      QString("where first = %1 and t1 = %2 and t2 = %3 and relation = %4")
      .arg(Uuid                                                           )
      .arg(Types::Schedule                                                )
      .arg(Types::Execution                                               )
      .arg(Groups::Subordination                                          )
    )                                                                     ;
    SC.Query(Q)                                                           ;
    Q = SC.sql.InsertInto(TABLES(Groups)                                  ,
          5,"first","second","t1","t2","relation"                       ) ;
    SC .Prepare ( Q                                                     ) ;
    SC .Bind    ( "first"    , Uuid                                     ) ;
    SC .Bind    ( "second"   , euid                                     ) ;
    SC .Bind    ( "t1"       , Types::Schedule                          ) ;
    SC .Bind    ( "t2"       , Types::Execution                         ) ;
    SC .Bind    ( "relation" , Groups::Subordination                    ) ;
    SC .Exec    (                                                       ) ;
  LeaveSQL         ( SC , plan->sql          )                            ;
  DeleteGadgets ( )                                                       ;
  update        ( )                                                       ;
  Alert ( Done )                                                          ;
}

void N::VcfGantt::DropChanged(void)
{
  switch (EventType)                  {
    case Audio                        :
      setDropFlag(DropText    ,false) ;
      setDropFlag(DropUrls    ,false) ;
      setDropFlag(DropHtml    ,false) ;
      setDropFlag(DropAudio   ,true ) ;
      setDropFlag(DropVideo   ,false) ;
      setDropFlag(DropAlbum   ,false) ;
      setDropFlag(DropURIs    ,false) ;
      setDropFlag(DropBookmark,false) ;
    break                             ;
    case Video                        :
      setDropFlag(DropText    ,false) ;
      setDropFlag(DropUrls    ,false) ;
      setDropFlag(DropHtml    ,false) ;
      setDropFlag(DropAudio   ,false) ;
      setDropFlag(DropVideo   ,true ) ;
      setDropFlag(DropAlbum   ,true ) ;
      setDropFlag(DropURIs    ,false) ;
      setDropFlag(DropBookmark,false) ;
    break                             ;
    case Period                       :
    case Record                       :
    case Meeting                      :
    case Automation                   :
    case Operation                    :
    case Sync                         :
    case Download                     :
    case Upload                       :
    case TV                           :
    case Programming                  :
    case Backup                       :
    case FileUpdate                   :
    case N::Notify                    :
    case Cooking                      :
    case Sleep                        :
    case Housework                    :
    case Shopping                     :
    case Communication                :
    case Others                       :
      setDropFlag(DropText    ,false) ;
      setDropFlag(DropUrls    ,false) ;
      setDropFlag(DropHtml    ,false) ;
      setDropFlag(DropAudio   ,false) ;
      setDropFlag(DropVideo   ,false) ;
      setDropFlag(DropAlbum   ,false) ;
      setDropFlag(DropURIs    ,false) ;
      setDropFlag(DropBookmark,false) ;
    break                             ;
    default                           :
    break                             ;
  }                                   ;
}
