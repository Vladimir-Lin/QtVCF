#include <qtvcf.h>

#define FONTID 4

N::VcfTask:: VcfTask   ( VcfHeadParaments  )
           : VcfBlocks ( parent , item , p )
{
  Configure ( ) ;
}

N::VcfTask::~VcfTask(void)
{
}

void N::VcfTask::Configure(void)
{
  Transform . reset (                                      ) ;
  ////////////////////////////////////////////////////////////
  Editable           = true                                  ;
  Borders [ Left   ] = 0.10                                  ;
  Borders [ Top    ] = 0.10                                  ;
  Borders [ Right  ] = 0.10                                  ;
  Borders [ Bottom ] = 0.10                                  ;
  ////////////////////////////////////////////////////////////
  setFlag            ( ItemIsMovable           ,true       ) ;
  setFlag            ( ItemIsSelectable        ,true       ) ;
  setFlag            ( ItemIsFocusable         ,true       ) ;
  setFlag            ( ItemClipsToShape        ,false      ) ;
  setFlag            ( ItemClipsChildrenToShape,true       ) ;
  ////////////////////////////////////////////////////////////
  Painter . addMap   ( "Default" , 0                       ) ;
  Painter . addPen   (      0 , QColor ( 255 , 192 , 192 ) ) ;
  Painter . addPen   (      1 , QColor ( 255 , 224 , 224 ) ) ;
  Painter . addPen   ( FONTID , QColor (   0 ,   0 ,   0 ) ) ;
  Painter . addBrush (      0 , QColor ( 255 , 240 , 240 ) ) ;
  Painter . addBrush (      1 , QColor ( 255 , 252 , 252 ) ) ;
  Painter . addBrush ( FONTID , QColor (   0 ,   0 ,   0 ) ) ;
}

void N::VcfTask::Paint(QPainter * p,QRectF Region,bool clip,bool color)
{
  RenderAppearance ( p , Region , clip , color ) ;
  RenderIcon       ( p , Region , clip , color ) ;
  RenderObjects    ( p , Region , clip , color ) ;
  RenderParagraph  ( p , Region , clip , color ) ;
  RenderDocument   ( p , Region , clip , color ) ;
}

void N::VcfTask::RenderAppearance(QPainter * p,QRectF,bool,bool)
{
  PaintPathes ( p ) ;
}

void N::VcfTask::RenderIcon(QPainter * p,QRectF region,bool clip,bool color)
{ Q_UNUSED ( region )                     ;
  Q_UNUSED ( clip   )                     ;
  Q_UNUSED ( color  )                     ;
  QImage I                                ;
  switch (nodeType)                       {
    case Tasks::Root                      :
    return                                ;
    case Tasks::Child                     :
    return                                ;
    case Tasks::Heir                      :
    return                                ;
    case Tasks::Prerequiste               :
    return                                ;
    case Tasks::Undecided                 :
      I = QImage(":/images/undecide.png") ;
    break                                 ;
    case Tasks::Standalone                :
      I = QImage(":/images/alone.png"   ) ;
    break                                 ;
    case Tasks::Framework                 :
      I = QImage(":/images/frame.png"   ) ;
    break                                 ;
    case Tasks::Decision                  :
      I = QImage(":/images/decision.png") ;
    break                                 ;
    case Tasks::Obsolete                  :
      I = QImage(":/images/delete.png"  ) ;
    break                                 ;
  }                                       ;
  p -> drawImage ( iconRect , I )         ;
}

QPointF N::VcfTask::Baseline(void)
{
  double ls = Options->LineSpace                     ;
  double dy = NameRect.bottom()                      ;
  if (nodes.count()>0)                               {
    for (int i=0;i<children.count();i++)             {
      VcfTask * task                                 ;
      task = VcfCasting(VcfTask,children[i])         ;
      if (NotNull(task))                             {
        QRectF SR = task->ScreenRect                 ;
        SR = task->mapToItem(this,SR).boundingRect() ;
        SR = Options->Standard(SR)                   ;
        if (SR.bottom()>dy) dy = SR.bottom()         ;
      }                                              ;
    }                                                ;
  } else                                             {
    dy += Borders [ Bottom ]                         ;
  }                                                  ;
  dy += ls                                           ;
  return QPointF ( Borders [ Left ] , dy )           ;
}

void N::VcfTask::Arrange(void)
{
  if (IsNull(Options)) return                            ;
  Painter . fonts[FONTID] . setDPI ( Options->DPI )      ;
  ////////////////////////////////////////////////////////
  bool icon = false                                      ;
  switch (nodeType)                                      {
    case Tasks::Undecided                                :
      icon = true                                        ;
      setPenColor   (1,QColor(224,224,224))              ;
      setBrushColor (1,QColor(255,255,255))              ;
    break                                                ;
    case Tasks::Standalone                               :
      icon = true                                        ;
      setPenColor   (1,QColor(224,224,255))              ;
      setBrushColor (1,QColor(252,252,255))              ;
    break                                                ;
    case Tasks::Framework                                :
      icon = true                                        ;
    break                                                ;
    case Tasks::Decision                                 :
      icon = true                                        ;
      setPenColor   (1,QColor(224,255,224))              ;
      setBrushColor (1,QColor(252,255,252))              ;
    break                                                ;
    case Tasks::Obsolete                                 :
      icon = true                                        ;
      setPenColor   (1,QColor(224,224,224))              ;
      setBrushColor (1,QColor(240,240,240))              ;
    break                                                ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  double        ls  = Options->LineSpace                 ;
  double        LS  = ls * 2                             ;
  QRectF        SR  = ScreenRect                         ;
  QFontMetricsF FMF = Painter.FontMetrics ( FONTID )     ;
  QRectF        RT  = FMF    .boundingRect( name   )     ;
  SR = Options -> Standard ( SR )                        ;
  RT = Options -> Standard ( RT )                        ;
  ////////////////////////////////////////////////////////
  QRectF TX                                              ;
  TX . setLeft   ( Borders   [ Left   ]            )     ;
  TX . setTop    ( Borders   [ Top    ]            )     ;
  TX . setWidth  ( RT.width  (        ) + LS       )     ;
  TX . setHeight ( RT.height (        ) + LS       )     ;
  ////////////////////////////////////////////////////////
  NameRect = TX                                          ;
  if (icon)                                              {
    QRectF IX                                            ;
    IX       . setLeft    ( TX . left   ()      )        ;
    IX       . setTop     ( TX . top    ()      )        ;
    IX       . setWidth   ( TX . height ()      )        ;
    IX       . setHeight  ( TX . height ()      )        ;
    NameRect . setLeft    ( IX . right  () + ls )        ;
    NameRect . setWidth   ( TX . width  ()      )        ;
    iconRect = toPaper    ( IX                  )        ;
  }                                                      ;
  ParagraphRect = toPaper ( NameRect            )        ;
  ////////////////////////////////////////////////////////
  PaperRect . setRight  ( NameRect . right  ()           +
                          Borders  [ Right   ]         ) ;
  PaperRect . setBottom ( NameRect . bottom ()           +
                          Borders  [ Bottom  ]         ) ;
  ////////////////////////////////////////////////////////
  QPainterPath path                                      ;
  QRectF R                                               ;
  R = Options->Region(PaperRect)                         ;
  if (expanded && children.count()>0)                    {
    double xc = 0                                        ;
    double yc = 0                                        ;
    for (int i=0;i<children.count();i++)                 {
      QRectF SC = children[i]->ScreenRect                ;
      SC = children[i]->mapToItem(this,SC).boundingRect() ;
      if (SC . right  () > xc) xc = SC . right        () ;
      if (SC . bottom () > yc) yc = SC . bottom       () ;
    }                                                    ;
    QPointF XP(xc,yc)                                    ;
    XP = Options->Standard(XP)                           ;
    XP . setX ( XP.x() + Borders[Right ] )               ;
    XP . setY ( XP.y() + Borders[Bottom] )               ;
    if (XP.x()  > PaperRect . right ())                  {
      PaperRect . setRight  (XP.x())                     ;
    }                                                    ;
    if (XP.y()  > PaperRect . bottom())                  {
      PaperRect . setBottom (XP.y())                     ;
    }                                                    ;
    XP = PaperRect . bottomRight (    )                  ;
    XP = Options  -> position    ( XP )                  ;
    QPolygonF PF                                         ;
    PF << R . topLeft     ()                             ;
    PF << R . topRight    ()                             ;
    PF << R . bottomRight ()                             ;
    PF << QPointF(XP.x(),PF[2].y())                      ;
    PF << XP                                             ;
    PF << QPointF(PF[0].x(),XP.y())                      ;
    PF << PF[0]                                          ;
    path.addPolygon ( PF )                               ;
  } else                                                 {
    path.addRect    ( R  )                               ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  Painter.pathes [1] = path                              ;
  EnablePath    ( 1 , true )                             ;
  MergePathes   ( 0        )                             ;
  ScreenRect = boundingRect()                            ;
}

bool N::VcfTask::FocusIn(void)
{
  Alert ( Click ) ;
  return true     ;
}

bool N::VcfTask::FocusOut(void)
{
  return false ;
}

void N::VcfTask::Hovering(QPointF pos)
{
}

bool N::VcfTask::AttemptMelt(VcfLinker * Linker,QPointF base)
{
  bool melt = false          ;
  if (Linker->isFirst(this)) {
    Linker->Melts[0] = base  ;
    melt = true              ;
  }                          ;
  if (Linker->isEnd  (this)) {
    Linker->Melts[1] = base  ;
    melt = true              ;
  }                          ;
  return melt                ;
}

bool N::VcfTask::AllowMelts(int side)
{
  return true ;
}
