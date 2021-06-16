#include <qtvcf.h>

N::VcfMind:: VcfMind   ( VcfHeadParaments  )
           : VcfBlocks ( parent , item , p )
{
  Configure ( ) ;
}

N::VcfMind::~VcfMind(void)
{
}

enum N::VcfNode::EditWays N::VcfMind::CastEditing(void)
{
  if (!Editable             ) return EditNothing ;
  if (!Markers.contains(999))                    {
    if (Scaling) return ScaleEditing             ;
    return EditNothing                           ;
  }                                              ;
  return (enum EditWays)Markers[999]             ;
}

void N::VcfMind::Configure(void)
{
  setFlag ( ItemIsMovable , false ) ;
}

void N::VcfMind::setRange(QRectF rect)
{
  VcfBlocks :: setRange       ( rect                     ) ;
  VcfGrid * grid = CreateGrid ( PaperRect                ) ;
  grid -> setFlag             ( ItemIsMovable    , false ) ;
  grid -> setFlag             ( ItemIsSelectable , false ) ;
  grid -> setFlag             ( ItemIsFocusable  , false ) ;
  emit Append                 ( grid             , this  ) ;
}

bool N::VcfMind::FocusIn(void)
{
  Alert ( Click ) ;
  return true     ;
}

bool N::VcfMind::FocusOut(void)
{
  return false ;
}

void N::VcfMind::Initialize(void)
{
  FetchFont  (  0 , "Default"    ) ;
  FetchPen   ( 11 , "TransPen"   ) ;
  FetchBrush ( 11 , "TransBrush" ) ;
}

void N::VcfMind::connectPressEvent(QGraphicsSceneMouseEvent * event)
{
  if (IsNull(attemptLink))                     {
    QGraphicsItem :: mousePressEvent ( event ) ;
    return                                     ;
  }                                            ;
  event -> accept ( )                          ;
}

void N::VcfMind::connectMoveEvent(QGraphicsSceneMouseEvent * event)
{
  if (IsNull(attemptLink))               {
    QGraphicsItem::mouseMoveEvent(event) ;
    return                               ;
  }                                      ;
  QPointF pos = event->pos (     )       ;
  QPointF s = alignPos     ( pos )       ;
  moveConnection           ( s   )       ;
  event -> accept          (     )       ;
}

void N::VcfMind::connectReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  QGraphicsItem::mouseReleaseEvent(event) ;
  QPointF pos = event->pos (     )        ;
  QPointF s = alignPos     ( pos )        ;
  endConnection            ( s   )        ;
}

bool N::VcfMind::doubleClicked(QPointF pos)
{
  if (!Editable) return false                                  ;
  QPointF s = alignPos(pos)                                    ;
  QPointF r (4.0,0.70)                                         ;
  QPointF h = r                                                ;
  h *= 0.5                                                     ;
  s -= h                                                       ;
  if (s.x()<PaperRect.left()) s.setX(PaperRect.left())         ;
  if (s.y()<PaperRect.top ()) s.setY(PaperRect.top ())         ;
  QRectF R(s.x(),s.y(),r.x(),r.y())                            ;
  VcfBlock * block = CreateBlock ( R )                         ;
  block -> Painter . fonts   [  4 ] = Painter . fonts   [  0 ] ;
  block -> Painter . pens    [ 11 ] = Painter . pens    [ 11 ] ;
  block -> Painter . brushes [ 11 ] = Painter . brushes [ 11 ] ;
  emit Append ( block , this  )                                ;
  block -> Mode = 0                                            ;
  block -> EnterEditor ( )                                     ;
  return true                                                  ;
}

void N::VcfMind::BlockUpdated(VcfBlock * block,int item)
{
  switch (item)                                             {
    case 0                                                  :
      if (block->name.length()<=0)                          {
        emit Remove ( block )                               ;
      } else                                                {
        block -> Mode = 1                                   ;
        block -> Compactified ( )                           ;
        block -> FocusOut     ( )                           ;
        block -> setZValue (0.25)                           ;
        connect(block,SIGNAL(Menu     (VcfNode*,QPointF))   ,
                this ,SLOT  (BlockMenu(VcfNode*,QPointF)) ) ;
        nodes << block                                      ;
      }                                                     ;
    break                                                   ;
  }                                                         ;
}

bool N::VcfMind::Menu(QPointF pos)
{
  VcfScopedMenu ( mm )                ;
  QAction    * aa = NULL              ;
  EditMenu       ( mm , pos )         ;
  AdjustmentMenu ( mm , pos )         ;
  mm . setFont   ( plan     )         ;
  aa = mm . exec (          )         ;
  if (IsNull     (aa   )) return true ;
  if (MenuProcess(aa,mm)) return true ;
  return true                         ;
}

void N::VcfMind::EditMenu(MenuManager & menu,QPointF pos)
{
  QMenu * ma = NULL                       ;
  /////////////////////////////////////////
  ma = menu.addMenu(tr("Edit"))           ;
  /////////////////////////////////////////
  menu . add ( ma , 801 , tr("Delete" ) ) ;
}

void N::VcfMind::ConnectMenu(VcfNode * node,QPointF ips,QPointF pos,MenuManager & menu)
{
  QMenu           * ma       = NULL             ;
  ///////////////////////////////////////////////
  ma = menu.addMenu(tr("Connections"))          ;
  ///////////////////////////////////////////////
  menu . add ( ma , 501 , tr("Hardwired"    ) ) ;
  menu . add ( ma , 502 , tr("Next node"    ) ) ;
  menu . add ( ma , 503 , tr("Previous node") ) ;
  menu . add ( ma , 504 , tr("Bidirectional") ) ;
}

void N::VcfMind::BlockMenu(VcfNode * node,QPointF pos)
{
  VcfScopedMenu ( mm )                     ;
  QPointF mps = node->mapToItem(this,pos)  ;
  QAction    * aa = NULL                   ;
  ConnectMenu          ( node,mps,pos,mm ) ;
  node->EditMenu       ( mm , pos        ) ;
  node->RelationMenu   ( mm , pos        ) ;
  node->AdjustmentMenu ( mm , pos        ) ;
  mm . setFont   ( plan     )              ;
  aa = mm . exec (          )              ;
  if (IsNull     (aa   )) return           ;
  switch (mm[aa])                          {
    case 501                               :
      startConnection ( node , mps , 1 )   ;
      Markers[999] = (int)ConnectEditing   ;
      grabMouse()                          ;
      node -> FocusIn ()                   ;
    break                                  ;
    case 502                               :
      startConnection ( node , mps , 2 )   ;
      Markers[999] = (int)ConnectEditing   ;
      grabMouse()                          ;
      node -> FocusIn ()                   ;
    break                                  ;
    case 503                               :
      startConnection ( node , mps , 3 )   ;
      Markers[999] = (int)ConnectEditing   ;
      grabMouse()                          ;
      node -> FocusIn ()                   ;
    break                                  ;
    case 504                               :
      startConnection ( node , mps , 4 )   ;
      Markers[999] = (int)ConnectEditing   ;
      grabMouse()                          ;
      node -> FocusIn ()                   ;
    break                                  ;
    default                                :
      node->MenuProcess(aa,mm)             ;
    break                                  ;
  }                                        ;
}

bool N::VcfMind::MenuProcess(QAction * action,MenuManager & menu)
{
  switch (menu[action])                               {
    case 901                                          :
      Editable = action -> isChecked ( )              ;
    break                                             ;
    case 902                                          :
      setFlag ( ItemIsMovable , action->isChecked() ) ;
    break                                             ;
    case 903                                          :
      Scaling = action->isChecked()                   ;
    return true                                       ;
  }                                                   ;
  return false                                        ;
}
