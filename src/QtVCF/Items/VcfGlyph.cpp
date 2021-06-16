#include <qtvcf.h>

#define GlyphMagic 0x12441234

N::VcfGlyph:: VcfGlyph  ( VcfHeadParaments  )
            : VcfCanvas ( parent , item , p )
{
  Configure ( ) ;
}

N::VcfGlyph::~VcfGlyph(void)
{
}

void N::VcfGlyph::Configure(void)
{
  Mode = Border                                          ;
  ////////////////////////////////////////////////////////
  setFlag      ( ItemIsMovable                 , true  ) ;
  setFlag      ( ItemIsSelectable              , true  ) ;
  setFlag      ( ItemIsFocusable               , true  ) ;
  setFlag      ( ItemClipsToShape              , false ) ;
  setFlag      ( ItemClipsChildrenToShape      , true  ) ;
  setFlag      ( ItemSendsGeometryChanges      , true  ) ;
  setFlag      ( ItemSendsScenePositionChanges , true  ) ;
  ////////////////////////////////////////////////////////
  setDropFlag       ( DropPen   , true                 ) ;
  setDropFlag       ( DropBrush , true                 ) ;
  setDropFlag       ( DropFont  , true                 ) ;
}

void N::VcfGlyph::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
  if (GlyphMenu(event->pos())) event->accept() ;
  else VcfCanvas::contextMenuEvent(event)      ;
}

void N::VcfGlyph::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
  if (doubleClicked(event->pos())) event->accept() ;
  else QGraphicsItem::mouseDoubleClickEvent(event) ;
}

void N::VcfGlyph::Paint(QPainter * painter,QRectF Region,bool clip,bool color)
{
  VcfCanvas::Paint(painter,Region,clip,color) ;
}

bool N::VcfGlyph::doubleClicked(QPointF pos)
{
  EnterEditor ( ) ;
  return true     ;
}

void N::VcfGlyph::setText(QString text)
{
  Text = text            ;
  ClearGlyph    (      ) ;
  GenerateGlyph ( Text ) ;
}

QRectF N::VcfGlyph::textRect(void)
{
  QRectF T (0,0,4,0.6)           ;
  if (IsNull(Options)) return T  ;
  T = ScreenRect                 ;
  T = Options -> Standard ( T )  ;
  T = QRectF(T.left  ()+0.1      ,
             T.bottom()-0.7      ,
             T.width ()-0.2      ,
                        0.6    ) ;
  return Options -> Region ( T ) ;
}

void N::VcfGlyph::EnterEditor(void)
{
  Editing = true                              ;
  QLineEdit * line = NewLineEdit ( 4 )        ;
  line -> setText ( Text )                    ;
  if (Painter.fonts.contains(4))              {
    Painter.fonts[4].setDPI( Options -> DPI ) ;
    line -> setFont ( Painter . fonts [ 4 ] ) ;
  }                                           ;
  QRectF NR  = textRect    (           )      ;
  Proxys[4] -> setGeometry ( NR        )      ;
  line      -> setFrame    ( true      )      ;
  connect(line,SIGNAL(editingFinished())      ,
          this,SLOT  (nameFinished   ())    ) ;
  line -> setFocus ( Qt :: TabFocusReason   ) ;
}

void N::VcfGlyph::nameFinished (void)
{
  QLineEdit * line = Casting(QLineEdit,Widgets[4]) ;
  if (NotNull(line)) Text = line->text()           ;
  DeleteGadgets (      )                           ;
  Editing = false                                  ;
  ClearGlyph    (      )                           ;
  GenerateGlyph ( Text )                           ;
}

bool N::VcfGlyph::ResizeFinish(QGraphicsSceneMouseEvent * event)
{
  if (Markers[0]==0) return false            ;
  PaperRect  = Options->Standard(ScreenRect) ;
  Markers[0] = 0                             ;
  setCursor ( Qt::ArrowCursor )              ;
  update    (                 )              ;
  if (Text.length()>0)                       {
    ClearGlyph    (      )                   ;
    GenerateGlyph ( Text )                   ;
  }                                          ;
  return true                                ;
}

void N::VcfGlyph::ClearGlyph(void)
{
  VcfItems gs                                   ;
  for (int i=0;i<Related.count();i++)           {
    VcfItem * item = Related[i]                 ;
    if (Relations.contains(item))               {
      int Mark = Relations[item]                ;
      if (Mark==GlyphMagic) gs << item          ;
    }                                           ;
  }                                             ;
  for (int i=0;i<gs.count();i++)                {
    VcfItem * item = gs[i]                      ;
    Related   . takeAt ( Related.indexOf(item)) ;
    Relations . remove ( item                 ) ;
    emit Remove        ( item                 ) ;
  };
}

void N::VcfGlyph::GenerateGlyph(QString text)
{
  if (IsNull(Options)) return              ;
  QFont   F = Painter.fonts[0]             ;
  QRectF  T = ScreenRect                   ;
  QPointF C                                ;
  QPointF Factor ( 1.0 , 1.0 )             ;
  Factor = Options -> Standard(Factor)     ;
  T = Options -> Standard ( T )            ;
  C . setX       ( T . left ()        )    ;
  T . setHeight  ( T . height() - 0.2 )    ;
  T = Options -> Region ( T )              ;
  F . setPixelSize ( T . height () )       ;
  QFontMetrics FMF ( F )                   ;
  QPointF V(0,FMF.ascent())                ;
  V = Options -> Standard ( V )            ;
  C . setY ( V.y() + 0.1 )                 ;
  QSizeF S ( Factor.x() , Factor.y() )     ;
  for (int i=0;i<text.length();i++)        {
    QRectF    B ( 0 , 0 , 0 , 0 )          ;
    QString   t = text.at(i)               ;
    Contours c                             ;
    c  = Graphics::Glyph(F,t)              ;
    C += QPointF(0.1,0.0)                  ;
    for (int j=0;j<c.count();j++)          {
      QRectF X                             ;
      VcfChild ( VcfContours , vc )        ;
      Contour b = c[j]                     ;
      b *= S                               ;
      b += C                               ;
      X = b . boundingRect (   )           ;
      B = B . united       ( X )           ;
      vc -> contour = b                    ;
      vc -> setOptions ( *Options , true ) ;
      vc -> setZValue  ( 0.75f           ) ;
      vc -> setOpacity ( 1.00f           ) ;
      vc -> show       (                 ) ;
      vc -> Prepare    ( false , false   ) ;
      Related      << vc                   ;
      Relations[vc] = GlyphMagic           ;
      emit Append      ( vc    , this    ) ;
    }                                      ;
    C += QPointF( B . width ( ) , 0 )      ;
  }                                        ;
}

bool N::VcfGlyph::GlyphMenu(QPointF)
{
  VcfScopedMenu ( mm )                          ;
  QAction    * aa                               ;
  bool filled = Painter.brushes.contains(1)     ;
  aa  = mm.add  ( 101 , tr("Border") )          ;
  aa -> setCheckable ( true          )          ;
  aa -> setChecked   ( Mode==Border  )          ;
  aa  = mm.add  ( 102 , tr("Fill"  ) )          ;
  aa -> setCheckable ( true          )          ;
  aa -> setChecked   ( filled        )          ;
  mm  . setFont      ( plan          )          ;
  aa  = mm . exec    (               )          ;
  if (IsNull(aa)) return true                   ;
  switch (mm[aa])                               {
    case 101                                    :
      Mode = aa->isChecked() ? Border : Empty   ;
      update ( )                                ;
    break                                       ;
    case 102                                    :
      filled = aa->isChecked()                  ;
      if (filled)                               {
        Painter.brushes[1] = Painter.brushes[0] ;
        for (int i=0;i<Related.count();i++)     {
          VcfItem * item = Related[i]           ;
          if (Relations.contains(item))         {
            int Mark = Relations[item]          ;
            if (Mark==GlyphMagic)               {
              item->Painter.brushes[1]          =
                    Painter.brushes[1]          ;
              item->update()                    ;
            }                                   ;
          }                                     ;
        }                                       ;
      } else                                    {
        Painter.brushes.remove(1)               ;
        for (int i=0;i<Related.count();i++)     {
          VcfItem * item = Related[i]           ;
          if (Relations.contains(item))         {
            int Mark = Relations[item]          ;
            if (Mark==GlyphMagic)               {
              item->Painter.brushes.remove(1)   ;
              item->update()                    ;
            }                                   ;
          }                                     ;
        }                                       ;
      }                                         ;
    break                                       ;
  }                                             ;
  return true                                   ;
}
