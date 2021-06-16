#include <qtvcf.h>

#define FONTID 4

N::VcfDecision:: VcfDecision ( VcfHeadParaments  )
               : VcfBlocks   ( parent , item , p )
{
  Configure ( ) ;
}

N::VcfDecision::~VcfDecision(void)
{
}

void N::VcfDecision::Configure(void)
{
  Transform . reset (                                 ) ;
  ///////////////////////////////////////////////////////
  Editable           = true                             ;
  Borders [ Left   ] = 0.20                             ;
  Borders [ Top    ] = 0.20                             ;
  Borders [ Right  ] = 0.20                             ;
  Borders [ Bottom ] = 0.20                             ;
  ///////////////////////////////////////////////////////
  Scaling = false                                       ;
  setFlag ( ItemIsMovable            , false )          ;
  setFlag ( ItemIsSelectable         , true  )          ;
  setFlag ( ItemIsFocusable          , true  )          ;
  setFlag ( ItemClipsToShape         , false )          ;
  setFlag ( ItemClipsChildrenToShape , false )          ;
  ///////////////////////////////////////////////////////
  Painter . addMap   ( "Default" , 0                  ) ;
  Painter . addPen   ( 0 , QColor ( 192 , 192 , 192 ) ) ;
  Painter . addPen   ( 1 , QColor (   0 ,   0 ,   0 ) ) ;
  Painter . addBrush ( 0 , QColor ( 240 , 240 , 240 ) ) ;
}

void N::VcfDecision::Paint(QPainter * painter,QRectF Region,bool clip,bool color)
{
}

void N::VcfDecision::Hovering(QPointF pos)
{
}

bool N::VcfDecision::FocusIn(void)
{
  Alert ( Click ) ;
  return true     ;
}

bool N::VcfDecision::FocusOut(void)
{
  return false ;
}

void N::VcfDecision::setUuid(QString n,SUID uu)
{
  uuid     = uu                                                ;
  node     = 0                                                 ;
  nodeType = VCF::Decision                                     ;
  name     = n                                                 ;
  setToolTip ( name )                                          ;
  //////////////////////////////////////////////////////////////
  GraphicsManager GM ( plan )                                  ;
  EnterSQL ( SC , plan->sql )                                  ;
    Painter . fonts [FONTID] = GM . GetFont ( SC , "Default" ) ;
  LeaveSQL ( SC , plan->sql )                                  ;
}

bool N::VcfDecision::doubleClicked(QPointF pos)
{
  if (!Editable) return false ;
  return true                 ;
}

void N::VcfDecision::DecisionMenu(MenuManager & menu,QPointF pos)
{
}

bool N::VcfDecision::Menu(QPointF pos)
{
  VcfScopedMenu  ( mm       ) ;
  QAction * aa                ;
  AdjustmentMenu ( mm , pos ) ;
  DecisionMenu   ( mm , pos ) ;
  mm . setFont   ( plan     ) ;
  aa = mm . exec (          ) ;
  if (IsNull(aa)) return true ;
  //
  return true                 ;
}
