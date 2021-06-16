#include <qtvcf.h>

N::VcfAction:: VcfAction ( VcfHeadParaments  )
             : VcfBlock  ( parent , item , p )
{
  Configure ( ) ;
}

N::VcfAction::~VcfAction(void)
{
}

void N::VcfAction::Configure(void)
{
  Transform . reset (                                   ) ;
  /////////////////////////////////////////////////////////
  Editable           = true                               ;
  Borders [ Left   ] = 0.10                               ;
  Borders [ Top    ] = 0.10                               ;
  Borders [ Right  ] = 0.10                               ;
  Borders [ Bottom ] = 0.10                               ;
  /////////////////////////////////////////////////////////
  setFlag            ( ItemIsMovable            , false ) ;
  setFlag            ( ItemIsSelectable         , true  ) ;
  setFlag            ( ItemIsFocusable          , true  ) ;
//  setFlag            ( ItemClipsToShape         , true  ) ;
//  setFlag            ( ItemClipsChildrenToShape , true  ) ;
  setFlag            ( ItemClipsToShape         , false ) ;
  setFlag            ( ItemClipsChildrenToShape , false ) ;
  /////////////////////////////////////////////////////////
  Painter . addMap   ( "Default"   , 0                  ) ;
  /////////////////////////////////////////////////////////
  setPenColor    (      0 , QColor ( 192 , 192 , 192 )  ) ;
  setBrushColor  (      0 , QColor ( 240 , 240 , 240 )  ) ;
}

void N::VcfAction::Paint(QPainter * p,QRectF region,bool clip,bool color)
{ Q_UNUSED ( p      ) ;
  Q_UNUSED ( region ) ;
  Q_UNUSED ( clip   ) ;
  Q_UNUSED ( color  ) ;
}
