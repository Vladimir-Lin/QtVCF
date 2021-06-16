#include <qtvcf.h>

N::VcfUi:: VcfUi     ( VcfHeadParaments  )
         : VcfBlocks ( parent , item , p )
{
  setFlag(ItemIsMovable           ,true )               ;
  setFlag(ItemIsSelectable        ,true )               ;
  setFlag(ItemIsFocusable         ,true )               ;
  setFlag(ItemClipsToShape        ,false)               ;
  setFlag(ItemClipsChildrenToShape,false)               ;
  Painter . addMap   ( "Default" , 0                  ) ;
  Painter . addPen   ( 0 , QColor ( 192 , 192 , 192 ) ) ;
  Painter . addPen   ( 1 , QColor (   0 ,   0 ,   0 ) ) ;
  Painter . addBrush ( 0 , QColor ( 240 , 240 , 240 ) ) ;
}

N::VcfUi::~VcfUi(void)
{
}

void N::VcfUi::Paint(QPainter * painter,QRectF region,bool clip,bool color)
{ Q_UNUSED ( painter ) ;
  Q_UNUSED ( region  ) ;
  Q_UNUSED ( clip    ) ;
  Q_UNUSED ( color   ) ;
}
