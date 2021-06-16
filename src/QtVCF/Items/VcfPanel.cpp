#include <qtvcf.h>

N::VcfPanel:: VcfPanel  ( VcfHeadParaments  )
            : VcfCanvas ( parent , item , p )
            , Tool      ( NULL              )
{
  Printable = true                                      ;
  Scaling   = false                                     ;
  Editable  = true                                      ;
  setFlag(ItemIsMovable           ,true )               ;
  setFlag(ItemIsSelectable        ,true )               ;
  setFlag(ItemIsFocusable         ,true )               ;
  setFlag(ItemClipsToShape        ,false)               ;
  setFlag(ItemClipsChildrenToShape,false)               ;
  Painter . addMap   ( "Default" , 0                  ) ;
  Painter . addPen   ( 0 , QColor ( 192 , 192 , 192 ) ) ;
  Painter . addBrush ( 0 , QColor ( 240 , 240 , 240 ) ) ;
}

N::VcfPanel::~VcfPanel(void)
{
  if (IsNull(Options)  ) return ;
  if (!Options->Private) return ;
  delete Options                ;
  Options = NULL                ;
}

void N::VcfPanel::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
  if (Menu(event->pos())) event->accept() ;
  else VcfCanvas::contextMenuEvent(event) ;
}

QList<QAction *> N::VcfPanel::actions(void)
{
  QList<QAction *> act         ;
  if (IsNull(Tool)) return act ;
  return Tool -> actions ( )   ;
}

QToolBar * N::VcfPanel::ToolBar(void)
{
  return Tool ;
}

void N::VcfPanel::Attach(QToolBar * toolBar)
{
  ToolProxy = new QGraphicsProxyWidget (this)      ;
  if (IsNull(toolBar))                             {
    Tool = new QToolBar()                          ;
  } else                                           {
    Tool = toolBar                                 ;
  }                                                ;
  ToolProxy -> setWidget ( Tool )                  ;
  ToolProxy -> setZValue  (1.0)                    ;
  ToolProxy -> setOpacity (1.0)                    ;
  ToolProxy -> setFlag    (ItemIsMovable   ,false) ;
  ToolProxy -> setFlag    (ItemIsSelectable,true ) ;
  ToolProxy -> setFlag    (ItemIsFocusable ,true ) ;
}

void N::VcfPanel::setRange(QRectF r)
{
  QPointF C = r . topLeft      (            ) ;
  QRectF  R( 0 , 0 , r.width() , r.height() ) ;
  VcfRectangle :: setPos       ( C          ) ;
  VcfRectangle :: setRect      ( R          ) ;
  if (IsNull(ToolProxy)) return               ;
  QRectF SR                                   ;
  SR.setLeft   (ScreenRect.left   ()+1      ) ;
  SR.setTop    (ScreenRect.top    ()+1      ) ;
  SR.setRight  (ScreenRect.right  ()-2      ) ;
  SR.setBottom (ScreenRect.bottom ()-2      ) ;
  ToolProxy -> setGeometry     ( SR         ) ;
  if (IsNull(Tool)) return                    ;
  int SH = SR.height() - 2                    ;
  QSize IS ( SH , SH )                        ;
  Tool -> setIconSize ( IS )                  ;
}

bool N::VcfPanel::Menu(QPointF pos)
{
  return false ;
}
