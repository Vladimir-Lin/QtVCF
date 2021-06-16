#include <qtvcf.h>

N::VcfBlocks:: VcfBlocks   ( VcfHeadParaments   )
             : VcfBlock    ( parent , item , p  )
             , Graph       (                    )
             , alignGrid   (QSizeF(0.10f,0.10f) )
             , attemptLink (NULL                )
{
  Configure ( ) ;
}

N::VcfBlocks::~VcfBlocks(void)
{
}

QPointF N::VcfBlocks::alignPos(QPointF pos)
{
  QPointF s  = Options -> Standard ( pos  )       ;
  int     nx = s . x ( ) / alignGrid . width  ( ) ;
  int     ny = s . y ( ) / alignGrid . height ( ) ;
  QPointF g                                       ;
  g . setX ( alignGrid . width  () * nx )         ;
  g . setY ( alignGrid . height () * ny )         ;
  return g                                        ;
}

void N::VcfBlocks::Hovering(QPointF pos)
{
  if (IsNull(Options)) return               ;
  QPointF s = alignPos(pos)                 ;
  QString m = tr("Cursor at grid [%1,%2] cm")
              .arg(s.x()).arg(s.y())        ;
  plan->showMessage(m)                      ;
  VcfBlock::Hovering(pos)                   ;
}

enum N::VcfNode::EditWays N::VcfBlocks::CastEditing(void)
{
  if (!Editable             ) return EditNothing ;
  if (!Markers.contains(999))                    {
    if (Scaling) return ScaleEditing             ;
    return EditNothing                           ;
  }                                              ;
  return (enum EditWays)Markers[999]             ;
}

void N::VcfBlocks::Configure(void)
{
  setFlag ( ItemIsMovable , false ) ;
  expanded = true                   ;
}

void N::VcfBlocks::setRange(QRectF rect)
{
  VcfBlock :: setRange ( rect ) ;
}

N::VcfGrid * N::VcfBlocks::CreateGrid(QRectF Range)
{
  VcfGrid * Grid = new VcfGrid(GraphicsView(),this,plan)         ;
  Grid  -> Scaling  = false                                      ;
  Grid  -> Editable = false                                      ;
  Grid  -> Gap      = QSizeF ( 0.500 , 0.500                   ) ;
  Grid  -> Dot      = QSizeF ( 0.025 , 0.025                   ) ;
  Grid  -> setOptions ( *Options , true                        ) ;
  Grid  -> setRange   ( Range                                  ) ;
  Grid  -> setFlag    ( ItemNegativeZStacksBehindParent , true ) ;
  Grid  -> setZValue  ( -0.01f                                 ) ;
  Grid  -> setOpacity (  1.00f                                 ) ;
  Grid  -> CreatePath (                                        ) ;
  Grid  -> show       (                                        ) ;
  return Grid                                                    ;
}

N::VcfBlock * N::VcfBlocks::CreateBlock(QRectF Range)
{
  VcfBlock * block = new VcfBlock(GraphicsView(),this,plan) ;
  block  -> Scaling  = true                                 ;
  block  -> Editable = true                                 ;
  block  -> setOptions ( *Options , true             )      ;
  block  -> setRange   ( Range                       )      ;
  block  -> setZValue  ( 0.30f                       )      ;
  block  -> setOpacity ( 1.00f                       )      ;
  block  -> show       (                             )      ;
  connect(block,SIGNAL(Update      (VcfBlock*,int))         ,
          this ,SLOT  (BlockUpdated(VcfBlock*,int))  )      ;
  return block                                              ;
}

N::VcfLinker * N::VcfBlocks::CreateLinker(QPointF start,QPointF final)
{
  ControlPoint CP1 ( start.x() , start.y() )                   ;
  ControlPoint CP2 ( final.x() , final.y() )                   ;
  VcfLinker * linker = new VcfLinker(GraphicsView(),this,plan) ;
  linker -> Editable = true                                    ;
  linker -> setOptions      ( *Options , true                ) ;
  linker -> setZValue       ( 0.50f                          ) ;
  linker -> setOpacity      ( 1.00f                          ) ;
  linker -> contour . begin (                                ) ;
  linker -> contour . add   ( 1 , CP1                        ) ;
  linker -> contour . add   ( 2 , CP2                        ) ;
  linker -> contour . end   (                                ) ;
  linker -> show            (                                ) ;
  return linker                                                ;
}

N::VcfLinker * N::VcfBlocks::CreateLinker(VcfNode * node,QPointF start,QPointF final)
{
  ControlPoint CP1 ( start.x() , start.y() )                   ;
  ControlPoint CP2 ( final.x() , final.y() )                   ;
  VcfLinker * linker = new VcfLinker(GraphicsView(),node,plan) ;
  linker -> Editable = true                                    ;
  linker -> setOptions      ( *Options , true                ) ;
  linker -> setZValue       ( 0.50f                          ) ;
  linker -> setOpacity      ( 1.00f                          ) ;
  linker -> contour . begin (                                ) ;
  linker -> contour . add   ( 1 , CP1                        ) ;
  linker -> contour . add   ( 2 , CP2                        ) ;
  linker -> contour . end   (                                ) ;
  linker -> show            (                                ) ;
  return linker                                                ;
}

void N::VcfBlocks::startConnection(VcfNode * node,QPointF pos,int flags)
{
  QPoint  cps  = QCursor::pos()                     ;
  QPoint  gps  = GraphicsView()->mapFromGlobal(cps) ;
  QPointF sps  = GraphicsView()->mapToScene   (gps) ;
  QPointF eps  = mapFromScene                 (sps) ;
  QPointF pps  = Options->Standard            (eps) ;
  QPointF xps  = Options->Standard            (pos) ;
  ///////////////////////////////////////////////////
  switch (flags)                                    {
    case 1                                          :
      attemptLink  = CreateLinker ( xps , pps   )   ;
      attemptLink -> EnableArrow  ( 0   , false )   ;
      attemptLink -> EnableArrow  ( 1   , false )   ;
      attemptLink -> setFirst     ( node        )   ;
    break                                           ;
    case 2                                          :
      attemptLink  = CreateLinker ( xps , pps   )   ;
      attemptLink -> EnableArrow  ( 0   , false )   ;
      attemptLink -> EnableArrow  ( 1   , true  )   ;
      attemptLink -> setFirst     ( node        )   ;
    break                                           ;
    case 3                                          :
      attemptLink  = CreateLinker ( pps , xps   )   ;
      attemptLink -> EnableArrow  ( 0   , false )   ;
      attemptLink -> EnableArrow  ( 1   , true  )   ;
      attemptLink -> setSecond    ( node        )   ;
    break                                           ;
    case 4                                          :
      attemptLink  = CreateLinker ( xps , pps   )   ;
      attemptLink -> EnableArrow  ( 0   , true  )   ;
      attemptLink -> EnableArrow  ( 1   , true  )   ;
      attemptLink -> setFirst     ( node        )   ;
    break                                           ;
  }                                                 ;
  ///////////////////////////////////////////////////
  attemptLink -> Prepare      ( false , false )     ;
  emit Append ( attemptLink , this )                ;
}

void N::VcfBlocks::moveConnection(QPointF pos)
{
  if (IsNull(attemptLink)) return        ;
  if (attemptLink->hasFirst())           {
    attemptLink->contour.points[2] = pos ;
  } else
  if (attemptLink->hasSecond())          {
    attemptLink->contour.points[1] = pos ;
  }                                      ;
  attemptLink->Prepare(false,false)      ;
  update ( )                             ;
}

void N::VcfBlocks::endConnection(QPointF pos)
{
  if (IsNull(attemptLink)) return        ;
  ///////////////////////////////////////////////////
  // Try to find out which one is going to connect
  ///////////////////////////////////////////////////
  if (attemptLink->hasFirst())           {
    attemptLink->contour.points[2] = pos ;
  } else
  if (attemptLink->hasSecond())          {
    attemptLink->contour.points[1] = pos ;
  }                                      ;
  attemptLink->Prepare(false,false)      ;
  update ( )                             ;
  attemptLink = NULL                     ;
  Markers . remove ( 999 )               ;
  ungrabMouse ( )                        ;
}

void N::VcfBlocks::BlockUpdated(VcfBlock * block,int item)
{
}

void N::VcfBlocks::addChildren(VcfNode * node)
{
  children << node   ;
  addNode   ( node ) ;
}

void N::VcfBlocks::ConnectMenu(VcfNode * node,QPointF ips,QPointF pos,MenuManager & menu)
{ Q_UNUSED ( node ) ;
  Q_UNUSED ( ips  ) ;
  Q_UNUSED ( pos  ) ;
  Q_UNUSED ( menu ) ;
}
