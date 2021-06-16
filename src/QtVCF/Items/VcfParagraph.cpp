#include <qtvcf.h>

// #define TRYOUT 1
#define FONTID   8
#define SELECTID 9

N::VcfParagraph:: VcfParagraph ( VcfHeadParaments  )
                : VcfBlock     ( parent , item , p )
                , Document     ( NULL              )
                , cursor       ( NULL              )
{
  Configure ( ) ;
}

N::VcfParagraph::~VcfParagraph(void)
{
}

void N::VcfParagraph::Configure(void)
{
  Transform . reset (                                    ) ;
  //////////////////////////////////////////////////////////
  Editable           = true                                ;
  TabReplacement     = "    "                              ;
  Borders [ Left   ] = 0.10                                ;
  Borders [ Top    ] = 0.10                                ;
  Borders [ Right  ] = 0.10                                ;
  Borders [ Bottom ] = 0.10                                ;
  //////////////////////////////////////////////////////////
  setFlag            ( ItemIsMovable            , false  ) ;
  setFlag            ( ItemIsSelectable         , true   ) ;
  setFlag            ( ItemIsFocusable          , true   ) ;
  setFlag            ( ItemClipsToShape         , true   ) ;
  setFlag            ( ItemClipsChildrenToShape , true   ) ;
  setFlag            ( ItemAcceptsInputMethod   , true   ) ;
  //////////////////////////////////////////////////////////
  Painter . addMap   ( "Default"   , 0                   ) ;
  Painter . addMap   ( "Paragraph" , FONTID              ) ;
  Painter . addMap   ( "Selection" , SELECTID            ) ;
  //////////////////////////////////////////////////////////
  setPenColor    ( 0        , QColor ( 224 , 224 , 224 ) ) ;
  setPenColor    ( FONTID   , QColor (   0 ,   0 ,   0 ) ) ;
  setPenColor    ( SELECTID , QColor (  96 ,  32 ,   0 ) ) ;
  setBrushColor  ( 0        , QColor ( 255 , 255 , 255 ) ) ;
  setBrushColor  ( FONTID   , QColor (   0 ,   0 ,   0 ) ) ;
  setBrushColor  ( SELECTID , QColor ( 252 , 240 , 192 ) ) ;
}

QString N::VcfParagraph::tabReplacement(void)
{
  return TabReplacement ;
}

void N::VcfParagraph::setTabReplacement(QString tab)
{
  TabReplacement = tab ;
}

//////////////////////////////////////////////////////////////////////////////

void N::VcfParagraph::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
  if (Menu(event->pos())) event->accept() ;
  else VcfNode::contextMenuEvent(event)   ;
}

void N::VcfParagraph::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
  QGraphicsItem :: mouseDoubleClickEvent ( event ) ;
}

//////////////////////////////////////////////////////////////////////////////

void N::VcfParagraph::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  if ( nEqual ( modifiers , Qt::NoModifier ) )   {
    if (IsMask(event->buttons(),Qt::LeftButton)) {
      textCursor.setPosition(0,event->pos())     ;
      CursorPressed   ( )                        ;
      event -> accept ( )                        ;
      return                                     ;
    }                                            ;
  } else                                         {
    if (IsMask(event->buttons(),Qt::LeftButton)) {
      textCursor.setPosition(10,event->pos())    ;
      CursorCombined()                           ;
      event -> accept ( )                        ;
      return                                     ;
    }                                            ;
  }                                              ;
  VcfNode::mousePressEvent(event)                ;
}

void N::VcfParagraph::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
  if ( nEqual ( modifiers , Qt::NoModifier ) )   {
    if (IsMask(event->buttons(),Qt::LeftButton)) {
      textCursor.setPosition(1,event->pos())     ;
      CursorMoving ( )                           ;
      event -> accept ( )                        ;
      return                                     ;
    }                                            ;
  }                                              ;
  VcfNode::mouseMoveEvent(event)                 ;
}

void N::VcfParagraph::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if ( nEqual (modifiers,Qt::NoModifier) ) {
    textCursor.setPosition(2,event->pos()) ;
    CursorReleased ( )                     ;
  }                                        ;
  VcfNode::mouseReleaseEvent(event)        ;
}

//////////////////////////////////////////////////////////////////////////////

void N::VcfParagraph::inputMethodEvent(QInputMethodEvent * event)
{// FunctionDebug ;
  if (insertIM(event->commitString(),event->preeditString())) {
    event->accept()                                           ;
    return                                                    ;
  }                                                           ;
  QGraphicsItem :: inputMethodEvent ( event )                 ;
}

QVariant N::VcfParagraph::inputMethodQuery(Qt::InputMethodQuery query) const
{
#ifdef TRYOUT
  switch (query) {
    case Qt::ImMicroFocus        : PlanDebug(30,"Qt::ImMicroFocus"       ); break ;
    case Qt::ImFont              : PlanDebug(30,"Qt::ImFont"             ); break ;
    case Qt::ImCursorPosition    : PlanDebug(30,"Qt::ImCursorPosition"   ); break ;
    case Qt::ImSurroundingText   : PlanDebug(30,"Qt::ImSurroundingText"  ); break ;
    case Qt::ImCurrentSelection  : PlanDebug(30,"Qt::ImCurrentSelection" ); break ;
    case Qt::ImMaximumTextLength : PlanDebug(30,"Qt::ImMaximumTextLength"); break ;
    case Qt::ImAnchorPosition    : PlanDebug(30,"Qt::ImAnchorPosition"   ); break ;
  };
#endif
  return QGraphicsItem::inputMethodQuery(query);
}

//////////////////////////////////////////////////////////////////////////////

bool N::VcfParagraph::sceneEvent(QEvent * event)
{
  if (nEqual(event->type(),QEvent::KeyPress))        {
    QKeyEvent * ke = static_cast<QKeyEvent *>(event) ;
    if (NotNull(ke))                                 {
      if (nEqual(ke->key(),Qt::Key_Tab)) return true ; // intercepts Tab key
    }                                                ;
  }                                                  ;
  return QGraphicsItem::sceneEvent(event)            ;
}

void N::VcfParagraph::keyPressEvent(QKeyEvent * event)
{
  modifiers = event->modifiers ()                ;
  if ( nEqual ( modifiers , Qt::NoModifier ) )   {
    switch (event->key())                        {
      case Qt::Key_Left                          :
        CursorLeft      ( )                      ;
        event->accept   ( )                      ;
      return                                     ;
      case Qt::Key_Right                         :
        CursorRight     ( )                      ;
        event->accept   ( )                      ;
      return                                     ;
      case Qt::Key_Up                            :
        CursorUp        ( )                      ;
        event->accept   ( )                      ;
      return                                     ;
      case Qt::Key_Down                          :
        CursorDown      ( )                      ;
        event->accept   ( )                      ;
      return                                     ;
      case Qt::Key_Home                          :
        CursorHome      ( )                      ;
        event->accept   ( )                      ;
      return                                     ;
      case Qt::Key_End                           :
        CursorEnd       ( )                      ;
        event->accept   ( )                      ;
      return                                     ;
      case Qt::Key_Delete                        :
        CursorDelete    ( )                      ;
        event->accept   ( )                      ;
      return                                     ;
      case Qt::Key_Enter                         :
      case Qt::Key_Return                        :
        CursorEnter     ( )                      ;
        event->accept   ( )                      ;
      return                                     ;
      case Qt::Key_Backspace                     :
        CursorBackspace ( )                      ;
        event->accept   ( )                      ;
      return                                     ;
      case Qt::Key_Tab                           :
        CursorTab       ( )                      ;
        event->accept   ( )                      ;
      return                                     ;
      case Qt::Key_Insert                        :
      case Qt::Key_PageUp                        :
      case Qt::Key_PageDown                      :
      case Qt::Key_Pause                         :
      case Qt::Key_ScrollLock                    :
      case Qt::Key_NumLock                       :
      case Qt::Key_CapsLock                      :
      case Qt::Key_Escape                        :
      case Qt::Key_F1                            :
      case Qt::Key_F2                            :
      case Qt::Key_F3                            :
      case Qt::Key_F4                            :
      case Qt::Key_F5                            :
      case Qt::Key_F6                            :
      case Qt::Key_F7                            :
      case Qt::Key_F8                            :
      case Qt::Key_F9                            :
      case Qt::Key_F10                           :
      case Qt::Key_F11                           :
      case Qt::Key_F12                           :
      break                                      ;
      default                                    :
        if (event->text().length()>0)            {
          if (insertText(event->text()))         {
            emit contentModified ( this )        ;
            update ( )                           ;
            return                               ;
          }                                      ;
        }                                        ;
      break                                      ;
    }                                            ;
  } else                                         {
    bool ctrl  = isModifier(Qt::ControlModifier) ;
    bool alt   = isModifier(Qt::AltModifier    ) ;
    bool shift = isModifier(Qt::ShiftModifier  ) ;
    if (!ctrl & !alt && shift)                   { // Shift
      switch (event->key())                      {
        case Qt::Key_Left                        : // Shift + Left
          ShiftLeft       ( )                    ;
          event->accept   ( )                    ;
        return                                   ;
        case Qt::Key_Right                       : // Shift + Right
          ShiftRight      ( )                    ;
          event->accept   ( )                    ;
        return                                   ;
        case Qt::Key_Up                          : // Shift + Up
          ShiftUp         ( )                    ;
          event->accept   ( )                    ;
        return                                   ;
        case Qt::Key_Down                        : // Shift + Down
          ShiftDown       ( )                    ;
          event->accept   ( )                    ;
        return                                   ;
        case Qt::Key_Home                        : // Shift + Home
          ShiftHome       ( )                    ;
          event->accept   ( )                    ;
        return                                   ;
        case Qt::Key_End                         : // Shift + End
          ShiftEnd        ( )                    ;
          event->accept   ( )                    ;
        return                                   ;
        case Qt::Key_Tab                         :
          ShiftTab        ( )                    ;
          event->accept   ( )                    ;
        return                                   ;
        case Qt::Key_Backspace                   :
          CursorBackspace ( )                    ;
          event->accept   ( )                    ;
        return                                   ;
        case Qt::Key_Delete                      :
        case Qt::Key_Enter                       :
        case Qt::Key_Return                      :
        case Qt::Key_Insert                      :
        case Qt::Key_PageUp                      :
        case Qt::Key_PageDown                    :
        case Qt::Key_Pause                       :
        case Qt::Key_ScrollLock                  :
        case Qt::Key_NumLock                     :
        case Qt::Key_CapsLock                    :
        case Qt::Key_Escape                      :
        case Qt::Key_F1                          :
        case Qt::Key_F2                          :
        case Qt::Key_F3                          :
        case Qt::Key_F4                          :
        case Qt::Key_F5                          :
        case Qt::Key_F6                          :
        case Qt::Key_F7                          :
        case Qt::Key_F8                          :
        case Qt::Key_F9                          :
        case Qt::Key_F10                         :
        case Qt::Key_F11                         :
        case Qt::Key_F12                         :
        break                                    ;
        default                                  :
          if (event->text().length()>0)          {
            if (insertText(event->text()))       {
              emit contentModified ( this )      ;
              update ( )                         ;
              return                             ;
            }                                    ;
          }                                      ;
        break                                    ;
      }                                          ;
    } else
    if (ctrl & !alt && !shift)                   { // Ctrl
      switch (event->key())                      {
        case Qt::Key_Backspace                   : // Ctrl + Backspace
          CtrlBackspace ( )                      ;
          event->accept ( )                      ;
        return                                   ;
        case Qt::Key_Left                        : // Ctrl + Left
        case Qt::Key_Right                       : // Ctrl + Right
        case Qt::Key_Up                          : // Ctrl + Up
        case Qt::Key_Down                        : // Ctrl + Down
        case Qt::Key_Home                        : // Ctrl + Home
        case Qt::Key_End                         : // Ctrl + End
        case Qt::Key_Tab                         :
        case Qt::Key_Delete                      :
        case Qt::Key_Enter                       :
        case Qt::Key_Return                      :
        case Qt::Key_Insert                      :
        case Qt::Key_PageUp                      :
        case Qt::Key_PageDown                    :
        case Qt::Key_Pause                       :
        case Qt::Key_ScrollLock                  :
        case Qt::Key_NumLock                     :
        case Qt::Key_CapsLock                    :
        case Qt::Key_Escape                      :
        case Qt::Key_F1                          :
        case Qt::Key_F2                          :
        case Qt::Key_F3                          :
        case Qt::Key_F4                          :
        case Qt::Key_F5                          :
        case Qt::Key_F6                          :
        case Qt::Key_F7                          :
        case Qt::Key_F8                          :
        case Qt::Key_F9                          :
        case Qt::Key_F10                         :
        case Qt::Key_F11                         :
        case Qt::Key_F12                         :
        break                                    ;
        default                                  :
        break                                    ;
      }                                          ;
    } else                                       {
    }                                            ;
  }                                              ;
  FunctionDebug                                  ;
  QGraphicsItem::keyPressEvent(event)            ;
}

void N::VcfParagraph::keyReleaseEvent(QKeyEvent * event)
{
  modifiers = event->modifiers ()       ;
  QGraphicsItem::keyReleaseEvent(event) ;
}

//////////////////////////////////////////////////////////////////////////////

QVariant N::VcfParagraph::itemChange(GraphicsItemChange change,const QVariant & value)
{
#ifdef TRYOUT
  FunctionDebug ;
  #define IC(name) case name : PlanDebug(30,#name); break
  switch (change)                          {
    IC(ItemEnabledChange                 ) ;
    IC(ItemEnabledHasChanged             ) ;
    IC(ItemMatrixChange                  ) ;
    IC(ItemPositionChange                ) ;
    IC(ItemPositionHasChanged            ) ;
    IC(ItemTransformChange               ) ;
    IC(ItemTransformHasChanged           ) ;
    IC(ItemRotationChange                ) ;
    IC(ItemRotationHasChanged            ) ;
    IC(ItemScaleChange                   ) ;
    IC(ItemScaleHasChanged               ) ;
    IC(ItemTransformOriginPointChange    ) ;
    IC(ItemTransformOriginPointHasChanged) ;
    IC(ItemSelectedChange                ) ;
    IC(ItemSelectedHasChanged            ) ;
    IC(ItemVisibleChange                 ) ;
    IC(ItemVisibleHasChanged             ) ;
    IC(ItemParentChange                  ) ;
    IC(ItemParentHasChanged              ) ;
    IC(ItemChildAddedChange              ) ;
    IC(ItemChildRemovedChange            ) ;
    IC(ItemSceneChange                   ) ;
    IC(ItemSceneHasChanged               ) ;
    IC(ItemCursorChange                  ) ;
    IC(ItemCursorHasChanged              ) ;
    IC(ItemToolTipChange                 ) ;
    IC(ItemToolTipHasChanged             ) ;
    IC(ItemFlagsChange                   ) ;
    IC(ItemFlagsHaveChanged              ) ;
    IC(ItemZValueChange                  ) ;
    IC(ItemZValueHasChanged              ) ;
    IC(ItemOpacityChange                 ) ;
    IC(ItemOpacityHasChanged             ) ;
    IC(ItemScenePositionHasChanged       ) ;
  }                                        ;
  #undef  IC
#endif
  return VcfNode::itemChange(change,value) ;
}
//////////////////////////////////////////////////////////////////////////////

void N::VcfParagraph::Hovering(QPointF pos)
{
  nDropOut ( !Editable       ) ;
  setCursor( Qt::IBeamCursor ) ;
}

void N::VcfParagraph::clearExclusions(void)
{
  Exclusions . clear () ;
}

void N::VcfParagraph::addExclusions(VcfItem * item,QRectF itemRect)
{
  Exclusions[item] = itemRect ;
}

//////////////////////////////////////////////////////////////////////////////

bool N::VcfParagraph::FocusIn(void)
{
  setTextCursor ( mousePosition ( )              ) ;
  LinkAction    ( Cut        , Cut            () ) ;
  LinkAction    ( Copy       , CopyToClipboard() ) ;
  LinkAction    ( Paste      , Paste          () ) ;
  LinkAction    ( SelectAll  , SelectAll      () ) ;
  LinkAction    ( SelectNone , ClearSelection () ) ;
  LinkAction    ( Delete     , CursorDelete   () ) ;
  Alert         ( Click                          ) ;
  return true                                      ;
}

bool N::VcfParagraph::FocusOut(void)
{
  textCursor . empty ( )              ;
  modifiers  = Qt::NoModifier         ;
  update ( )                          ;
  /////////////////////////////////////
  nKickOut ( IsNull(cursor) , false ) ;
  VcfCursor * c = cursor              ;
  cursor = NULL                       ;
  emit Remove(c)                      ;
  return false                        ;
}

//////////////////////////////////////////////////////////////////////////////

void N::VcfParagraph::paint(QPainter * p,const QStyleOptionGraphicsItem * option,QWidget * widget)
{
  QRectF Z = clipRect (                      ) ;
  Paint               ( p , Z , false , true ) ;
}

void N::VcfParagraph::Paint(QPainter * p,QRectF Region,bool clip,bool color)
{
  nDropOut     ( IsNull(plan    )                ) ;
  nDropOut     ( IsNull(Options )                ) ;
  nDropOut     ( IsNull(Document)                ) ;
  nDropOut     ( !Painter.fonts.contains(FONTID) ) ;
  pushPainters ( p                               ) ;
  RenderAll    ( p                               ) ;
  popPainters  ( p                               ) ;
}

QPainterPath N::VcfParagraph::visiblePath(void)
{
  QRectF SR = visibleRect()           ;
  QPainterPath SP                     ;
  SP . addRect ( SR )                 ;
  SP = SP . intersected ( Available ) ;
  return SP                           ;
}

void N::VcfParagraph::RenderAll(QPainter * p)
{
  QPainterPath          SP = visiblePath()       ;
  ////////////////////////////////////////////////
  QList<TextBlock   *> Blocks                    ;
  QList<TextSection *> Sections                  ;
  ////////////////////////////////////////////////
  pushPainters ( p                       )       ;
  p->fillPath  ( SP , Painter.brushes[0] )       ;
  Painter . setPainter ( p , "Default"   )       ;
  p->drawPath  ( SP                      )       ;
  popPainters  ( p                       )       ;
  ////////////////////////////////////////////////
  for (int i=0;i<Document->blockCount();i++)     {
    TextBlock * tb      = (*Document)[i]         ;
    bool        visible = true                   ;
    if ( tb->count()<=0    ) visible = false     ;
    if (!tb->intersects(SP)) visible = false     ;
    if (visible            ) Blocks << tb        ;
  }                                              ;
  nDropOut ( Blocks.count() <= 0 )               ;
  ////////////////////////////////////////////////
  for (int i=0;i<Blocks.count();i++)             {
    TextBlock * tb      = Blocks[i]              ;
    bool        visible = true                   ;
    if ( tb->count()<=0    ) visible = false     ;
    if (!tb->intersects(SP)) visible = false     ;
    if (visible) Sections << tb->sections        ;
  }                                              ;
  nDropOut ( Sections . count () <= 0 )          ;
  ////////////////////////////////////////////////
  for (int j=0;j<Sections.count();j++)           {
    TextSection * ts          = Sections[j]      ;
    bool          intersected = true             ;
    if (!ts->intersects(SP)) intersected = false ;
    if (intersected)                             {
      pushPainters ( p )                         ;
      ts->Render   ( p )                         ;
      popPainters  ( p )                         ;
    }                                            ;
  }                                              ;
}

bool N::VcfParagraph::isModifier(enum Qt::KeyboardModifier modifier)
{
  return IsMask ( modifiers , modifier ) ;
}

bool N::VcfParagraph::isCursorEditing(void)
{
  nKickOut ( !Editable                 , false ) ;
  nKickOut ( IsNull(Document         ) , false ) ;
  nKickOut ( !textCursor.isActivated() , false ) ;
  return true                                    ;
}

void N::VcfParagraph::LocateCursor(void)
{
  nDropOut ( IsNull(cursor) )          ;
  int     atp = textCursor . At        ;
  QPointF p   = textCursor.sectionAt() ;
  QRectF  ch  = cursor->ScreenRect     ;
  QPointF s(ch.width(),ch.height())    ;
  s /= 2                               ;
  p += s                               ;
  setTextCursor ( p )                  ;
  textCursor . At = atp                ;
}

void N::VcfParagraph::LocateAnchor(void)
{
  Document  -> clearSelection ( textCursor ) ;
  textCursor . Anchor         (            ) ;
}

QPointF N::VcfParagraph::CursorBottom(QPointF pos)
{
  qreal h = cursor->ScreenRect.height() ;
  h *= 0.8                              ;
  pos.setY(pos.y()+h)                   ;
  return pos                            ;
}

void N::VcfParagraph::CursorLeft(void)
{
  nDropOut ( !isCursorEditing() )               ;
  QPainterPath path = visiblePath()             ;
  bool         repaint = false                  ;
  bool         scroll  = false                  ;
  bool         updated = false                  ;
  QPointF      p                                ;
  if (textCursor.At>0)                          {
    p       = textCursor . Left ( )             ;
    updated = true                              ;
  } else                                        {
    TextSection * ts                            ;
    ts = Document->Previous(textCursor.Section) ;
    if (NotNull(ts)) textCursor.Last(ts)        ;
    if (textCursor.isActivated())               {
      p       = textCursor.sectionAt()          ;
      updated = true                            ;
    }                                           ;
  }                                             ;
  if (updated)                                  {
    bool selected = textCursor.Selected         ;
    int  atp      = textCursor.At               ;
    setTextCursor ( p )                         ;
    textCursor . At = atp                       ;
    LocateAnchor ( )                            ;
    scroll     = !path.contains(p)              ;
    if (scroll  ) repaint = true                ;
    if (selected) repaint = true                ;
  }                                             ;
  if (scroll ) emit scrollUp ( this , p )       ;
  if (repaint) update ( )                       ;
}

void N::VcfParagraph::CursorRight(void)
{
  nDropOut ( !isCursorEditing() )              ;
  QPainterPath path   = visiblePath()          ;
  int     length  = textCursor.sectionLength() ;
  bool    repaint = false                      ;
  bool    scroll  = false                      ;
  bool    added   = false                      ;
  QPointF p                                    ;
  if (textCursor.At<length)                    {
    p     = textCursor.Right()                 ;
    added = true                               ;
  } else                                       {
    TextSection * ts                           ;
    ts = Document->Next(textCursor.Section)    ;
    textCursor.Home(ts)                        ;
    if (textCursor.isActivated())              {
      p     = textCursor.sectionAt()           ;
      added = true                             ;
    }                                          ;
  }                                            ;
  if (added)                                   {
    bool selected = textCursor.Selected        ;
    int  atp      = textCursor.At              ;
    setTextCursor ( p )                        ;
    textCursor . At = atp                      ;
    LocateAnchor ( )                           ;
    p = CursorBottom(p)                        ;
    scroll = !path.contains(p)                 ;
    if (selected) repaint = true               ;
  }                                            ;
  if (scroll ) repaint = true                  ;
  if (scroll ) emit scrollDown ( this , p )    ;
  if (repaint) update ( )                      ;
}

void N::VcfParagraph::CursorUp(void)
{
  nDropOut ( !isCursorEditing() )             ;
  QPainterPath path = visiblePath()           ;
  TextSection * ts                            ;
  ts = Document->Previous(textCursor.Section) ;
  if (IsNull(ts)) return                      ;
  bool    selected = textCursor.Selected      ;
  bool    repaint  = false                    ;
  QRectF  r        = ts->Rectangle()          ;
  QPointF p                                   ;
  p = textCursor.sectionAt()                  ;
  p . setY(r.top()+0.001)                     ;
  setTextCursor ( p )                         ;
  LocateAnchor ( )                            ;
  if (!path.contains(p))                      {
    emit scrollUp(this,p)                     ;
    repaint = true                            ;
  }                                           ;
  if (selected) repaint = true                ;
  if (repaint ) update ( )                    ;
}

void N::VcfParagraph::CursorDown(void)
{
  nDropOut ( !isCursorEditing() )            ;
  QPainterPath path = visiblePath()          ;
  TextSection * ts                           ;
  ts = Document->Next(textCursor.Section)    ;
  if (IsNull(ts)) return                     ;
  bool    selected = textCursor.Selected     ;
  bool    repaint  = false                   ;
  QRectF  r        = ts->Rectangle()         ;
  QPointF p                                  ;
  p = textCursor.sectionAt()                 ;
  p . setY(r.top()+0.001)                    ;
  setTextCursor ( p )                        ;
  LocateAnchor ( )                           ;
  p = CursorBottom(p)                        ;
  if (!path.contains(p))                     {
    emit scrollDown(this,p)                  ;
    repaint = true                           ;
  }                                          ;
  if (selected) repaint = true               ;
  if (repaint ) update ( )                   ;
}

void N::VcfParagraph::CursorHome(void)
{
  nDropOut ( !isCursorEditing() )             ;
  bool    selected = textCursor.Selected      ;
  bool    repaint  = false                    ;
  QRectF  r = textCursor.Section->Rectangle() ;
  QPointF p                                   ;
  textCursor . At = 0                         ;
  p = textCursor.sectionAt()                  ;
  p . setY(r.top()+0.001)                     ;
  setTextCursor ( p )                         ;
  textCursor . At = 0                         ;
  LocateAnchor ( )                            ;
  if (selected) repaint = true                ;
  if (repaint ) update ( )                    ;
}

void N::VcfParagraph::CursorEnd(void)
{
  nDropOut ( !isCursorEditing() )             ;
  bool    selected = textCursor.Selected      ;
  bool    repaint  = false                    ;
  QRectF  r = textCursor.Section->Rectangle() ;
  int     atp = textCursor.sectionLength()    ;
  QPointF p                                   ;
  textCursor . At = atp                       ;
  p = textCursor.sectionAt()                  ;
  p . setY(r.top()+0.001)                     ;
  setTextCursor ( p )                         ;
  textCursor . At = atp                       ;
  LocateAnchor ( )                            ;
  if (selected) repaint = true                ;
  if (repaint ) update ( )                    ;
}

void N::VcfParagraph::CursorTab(void)
{
  nDropOut ( !isCursorEditing() ) ;
  if (insertText(TabReplacement)) {
    update               (      ) ;
    emit contentModified ( this ) ;
  }                               ;
}

void N::VcfParagraph::CursorDelete(void)
{
  nDropOut(!isCursorEditing()) ;
  Document->Delete(textCursor) ;
  ArrangeBlock         (    )  ;
  Arrange              (    )  ;
  LocateCursor         (    )  ;
  LocateAnchor         (    )  ;
  update               (    )  ;
  emit contentModified (this)  ;
}

void N::VcfParagraph::CursorEnter(void)
{
  nDropOut ( !isCursorEditing() )               ;
  TextCursor TC(textCursor)                     ;
  int bi = Document->indexOf(TC.Section->block) ;
  TextSection * tn = textCursor.Section         ;
  Document->Split(TC)                           ;
  Document->EnumSections()                      ;
  ArrangeBlock()                                ;
  TextBlock   * tb = (*Document)[bi+1]          ;
  TextSection * ts = NULL                       ;
  if (tb->count()>0) ts = tb->sections[0]       ;
  textCursor.Section = tn                       ;
  textCursor.Full    = tn->block                ;
  textCursor.At = 0                             ;
  Arrange      ( )                              ;
  textCursor.Section = ts                       ;
  textCursor.Full    = tb                       ;
  textCursor.At      = 0                        ;
  LocateCursor ( )                              ;
  LocateAnchor ( )                              ;
  emit contentModified ( this )                 ;
  update  ( )                                   ;
}

void N::VcfParagraph::CursorBackspace(void)
{
  nDropOut     ( !isCursorEditing() ) ;
  CursorLeft   (                    ) ;
  CursorDelete (                    ) ;
}

void N::VcfParagraph::ShiftLeft(void)
{
  nDropOut ( !isCursorEditing() )   ;
  QPainterPath path = visiblePath() ;
}

void N::VcfParagraph::ShiftRight(void)
{
  nDropOut ( !isCursorEditing() )      ;
  QPainterPath path    = visiblePath() ;
}

void N::VcfParagraph::ShiftUp(void)
{ FunctionDebug ;
  nDropOut ( !isCursorEditing() )   ;
  QPainterPath path = visiblePath() ;
}

void N::VcfParagraph::ShiftDown(void)
{ FunctionDebug ;
  nDropOut ( !isCursorEditing() )   ;
  QPainterPath path = visiblePath() ;
}

void N::VcfParagraph::ShiftHome(void)
{ FunctionDebug ;
  nDropOut ( !isCursorEditing() )   ;
  QPainterPath path = visiblePath() ;
}

void N::VcfParagraph::ShiftEnd(void)
{ FunctionDebug ;
  nDropOut ( !isCursorEditing() )   ;
  QPainterPath path = visiblePath() ;
}

void N::VcfParagraph::ShiftTab(void)
{ FunctionDebug ;
  nDropOut ( !isCursorEditing() )   ;
  QPainterPath path = visiblePath() ;
}

void N::VcfParagraph::CtrlBackspace(void)
{ FunctionDebug ;
  nDropOut ( !isCursorEditing() )   ;
  QPainterPath path = visiblePath() ;
}

void N::VcfParagraph::setTextCursor(QPointF pos)
{
  QPointF pfs = pos                               ;
  double  ph  = Painter.fonts[FONTID].pixelSize() ;
  double  ls  = Options->LineSpace                ;
  QPointF PS(ls,ls)                               ;
  PS  = Options -> position ( PS )                ;
  ph += PS.y()                                    ;
  QPointF PX(PS.x()/4,ph)                         ;
  PX = Options->Standard(PX)                      ;
  /////////////////////////////////////////////////
  TextSection * section = NULL                    ;
  textCursor . empty ( )                          ;
  if (NotNull(Document))                          {
    section = Document->contains(pfs)             ;
    if (NotNull(section))                         {
      QPointF corner = section->Locate(pfs)       ;
      pfs.setX(corner.x()           )             ;
      pfs.setY(section->position.y())             ;
      textCursor . Section = section              ;
      textCursor . At = section->cursorAt(pos)    ;
      if (textCursor.At<0)                        {
        textCursor.At = section->section.length() ;
      }                                           ;
    } else                                        {
      FocusOut()                                  ;
      return                                      ;
    }                                             ;
  } else                                          {
    FocusOut()                                    ;
    return                                        ;
  }                                               ;
  pfs  = Options->Standard(pfs)                   ;
  pfs . setY(pfs.y()-ls)                          ;
  /////////////////////////////////////////////////
  QSizeF SX(PX.x(),PX.y()+ls)                     ;
  QRectF CR                                       ;
  CR.setTopLeft(pfs)                              ;
  CR.setSize(SX)                                  ;
  /////////////////////////////////////////////////
  setCursorPosition ( CR )                        ;
}

void N::VcfParagraph::setCursorPosition(QRectF Range)
{
  if (IsNull(cursor))                                           {
    cursor = new VcfCursor     ( GraphicsView() , this , plan ) ;
    cursor -> Painter.addPen   ( 0 , QColor(  0,  0,255)      ) ;
    cursor -> Painter.addBrush ( 0 , QColor(128,128,255)      ) ;
    cursor -> setOptions       ( *Options , true              ) ;
    cursor -> setZValue        ( 0.50f                        ) ;
    cursor -> setOpacity       ( 1.00f                        ) ;
    cursor -> setRange         ( Range                        ) ;
    cursor -> show             (                              ) ;
    cursor -> setInterval      ( 500                          ) ;
    cursor -> Start            (                              ) ;
    emit Append                ( cursor , this                ) ;
  } else                                                        {
    cursor -> setRange         ( Range                        ) ;
  }                                                             ;
}

void N::VcfParagraph::CursorPressed(void)
{
  nDropOut ( !Editable        )               ;
  nDropOut ( IsNull(Document) )               ;
  bool selected = textCursor.Selected         ;
  setTextCursor(textCursor.Points[0])         ;
  if (selected)                               {
    Document -> clearSelection ( textCursor ) ;
  }                                           ;
  textCursor . Anchor          (            ) ;
  update ( )                                  ;
}

void N::VcfParagraph::CursorMoving(void)
{
  nDropOut ( !Editable        )                    ;
  nDropOut ( IsNull(Document) )                    ;
  QPointF       ps = textCursor.Points[0]          ;
  QPointF       pe = textCursor.Points[1]          ;
  TextSection * secti                              ;
  TextSection * esect                              ;
  int           SAT                                ;
  int           EAT                                ;
  secti = Document->contains(ps)                   ;
  esect = Document->contains(pe)                   ;
  setTextCursor ( pe )                             ;
  nDropOut ( IsNull(secti) )                       ;
  nDropOut ( IsNull(esect) )                       ;
  //////////////////////////////////////////////////
  SAT = secti->cursorAt(ps)                        ;
  EAT = esect->cursorAt(pe)                        ;
  if (SAT<0) SAT = secti->section.length()         ;
  if (EAT<0) EAT = esect->section.length()         ;
  Document->Select(textCursor,secti,SAT,esect,EAT) ;
  update ( )                                       ;
}

void N::VcfParagraph::CursorReleased(void)
{
  nDropOut ( !Editable        ) ;
  nDropOut ( IsNull(Document) ) ;
}

void N::VcfParagraph::CursorCombined(void)
// Shift + LeftButton Pressed
{
  nDropOut ( !Editable        )                    ;
  nDropOut ( IsNull(Document) )                    ;
  //////////////////////////////////////////////////
  bool ctrl  = isModifier(Qt::ControlModifier)     ;
  bool alt   = isModifier(Qt::AltModifier    )     ;
  bool shift = isModifier(Qt::ShiftModifier  )     ;
  //////////////////////////////////////////////////
  nDropOut ( ctrl )                                ;
  nDropOut ( alt  )                                ;
  nDropOut (!shift)                                ; // Shift + LeftButton only
  //////////////////////////////////////////////////
  QPointF       pe    = textCursor.Points[10]      ;
  TextSection * secti = textCursor.Section         ;
  TextSection * esect                              ;
  int           SAT   = textCursor.At              ;
  int           EAT   = textCursor.At              ;
  setTextCursor ( pe )                             ;
  esect = textCursor.Section                       ;
  EAT   = textCursor.At                            ;
  Document->Select(textCursor,secti,SAT,esect,EAT) ;
  update ( )                                       ;
}

QRectF N::VcfParagraph::SectionRect(QRectF rect)
{
  return rect ;
}

void N::VcfParagraph::Prepare(void)
{
  GraphicsManager GM( plan )                                   ;
  EnterSQL ( SC , plan->sql )                                  ;
    Painter . fonts [FONTID] = GM . GetFont ( SC , "Default" ) ;
  LeaveSQL ( SC , plan->sql )                                  ;
  if (Painter.fonts.contains(FONTID))                          {
    Painter . fonts [FONTID] . setDPI ( Options->DPI   )       ;
  }                                                            ;
}

int N::VcfParagraph::addPaper(QRectF Paper)
{
  QRectF CPer                   ;
  CPer = Options->Region(Paper) ;
  Composes << CPer              ;
  Available . addRect  ( CPer ) ;
  return Composes.count()       ;
}

void N::VcfParagraph::setDefaultSection(TextSection & NTS)
{
  NTS.font         = &Painter.fonts   [FONTID  ] ;
  NTS.pen          = &Painter.pens    [FONTID  ] ;
  NTS.brush        = &Painter.brushes [FONTID  ] ;
  NTS.selectPen    = &Painter.pens    [SELECTID] ;
  NTS.selectBrush  = &Painter.brushes [SELECTID] ;
  NTS.alignment    = Qt::AlignLeft               |
                     Qt::AlignVCenter            ;
  NTS.attributes   = 0                           ;
  NTS.area         = NTS . Measure ( )           ;
}

void N::VcfParagraph::BeginPartition(TextDocument * document)
{
  Document = document         ;
  nDropOut (IsNull(Document)) ;
  Document  -> Prepare ( )    ;
  textCursor . Booking ( )    ;
}

int N::VcfParagraph::Partition(void)
{
  double  ls   = Options->LineSpace                       ;
  double  ph   = Painter.fonts[FONTID].pixelSize()        ;
  double  dh   = ph                                       ;
  QPointF LW(Borders[Left ],Borders[Top   ])              ;
  QPointF EW(Borders[Right],Borders[Bottom])              ;
  QPointF PS(ls,ls)                                       ;
  PS  = Options -> position ( PS )                        ;
  LW  = Options -> position ( LW )                        ;
  EW  = Options -> position ( EW )                        ;
  /////////////////////////////////////////////////////////
  QRectF CR = Composes[textCursor.Page]                   ;
  double xp = CR.left  ()                                 ;
  double yp = CR.top   ()                                 ;
  double dw = CR.width ()                                 ;
  xp       += LW.x()                                      ;
  yp       += LW.y()                                      ;
  dw       -= LW.x()                                      ;
  dw       -= EW.x()                                      ;
  dh       += PS.y()                                      ;
  QSizeF WS(dw,ph)                                        ;
  /////////////////////////////////////////////////////////
  while (Document->contains(textCursor))                  {
    int length = Document->length(textCursor)             ;
    if (length>0)                                         {
      while (textCursor.Paragraph<length)                 {
        QPointF       position ( xp       , yp )          ;
        QRectF        RT       ( position , WS )          ;
        TextBlock   * block = (*Document)[textCursor]     ;
        TextSection * TS    = block->NewSection("")       ;
        TS->region     = SectionRect(RT)                  ;
        TS->section    = Document->Measure                (
                            textCursor                    ,
                            Painter.fonts [FONTID]        ,
                            RT                          ) ;
        TS->position   = position                         ;
        setDefaultSection ( *TS )                         ;
        textCursor.Paragraph += TS->length()              ;
        block->join(TS)                                   ;
        yp += dh                                          ;
        if ((yp+dh)>CR.bottom())                          {
          Document -> boundingRect(textCursor)            ;
          textCursor.Page++                               ;
          return textCursor.Page                          ;
        }                                                 ;
      }                                                   ;
      Document->boundingRect(textCursor)                  ;
    } else                                                {
      QPointF       position ( xp       , yp )            ;
      QRectF        RT       ( position , WS )            ;
      TextBlock   * block = (*Document)[textCursor]       ;
      TextSection * TS    = block->NewSection("")         ;
      TS -> region         = SectionRect(RT)              ;
      TS -> section        = ""                           ;
      TS -> position       = position                     ;
      setDefaultSection ( *TS )                           ;
      block->join(TS)                                     ;
      Document -> boundingRect(textCursor)                ;
      yp += dh                                            ;
      if ((yp+dh)>CR.bottom())                            {
        textCursor.Page++                                 ;
        return textCursor.Page                            ;
      }                                                   ;
    }                                                     ;
    textCursor.Block++                                    ;
    textCursor.Paragraph = 0                              ;
  }                                                       ;
  /////////////////////////////////////////////////////////
  return -1                                               ;
}

int N::VcfParagraph::atPage(QPointF pos)
{
  for (int i=0;i<Composes.count();i++) {
    QRectF R = Composes [ i ]          ;
    nKickOut ( R . contains(pos) , i ) ;
  }                                    ;
  return -1                            ;
}

bool N::VcfParagraph::ArrangeBlock(void)
{
  nKickOut ( IsNull(Document         ) , false )     ;
  nKickOut ( !textCursor.isActivated() , false )     ;
  ////////////////////////////////////////////////////
  arrangement = textCursor                           ;
  ////////////////////////////////////////////////////
  TextSection  * ts   = textCursor.Section           ;
  TextBlock    * tb   = ts->block                    ;
  QPointF        ps   = tb->position                 ;
  bool           done = false                        ;
  TextSection  * is   = NULL                         ;
  int            page = atPage(ps)                   ;
  int            npid = Document->blocks.indexOf(tb) ;
  bool           fina = true                         ;
  nKickOut ( page < 0 , false )                      ;
  nKickOut ( npid < 0 , false )                      ;
  locator    . lock ( )                              ;
  textCursor . Page = page                           ;
  ////////////////////////////////////////////////////
  TextBlock * tn   = (*Document)[npid+1]             ;
  QPointF     pt                                     ;
  if (NotNull(tn)) pt = tn->position                 ;
              else pt = tb->Rectangle().bottomLeft() ;
  textCursor.Paragraph = tb->ParagraphAt(textCursor) ;
  ////////////////////////////////////////////////////
  double  ls   = Options->LineSpace                  ;
  double  ph   = Painter.fonts[FONTID].pixelSize()   ;
  double  dh   = ph                                  ;
  QPointF LW(Borders[Left ],Borders[Top   ])         ;
  QPointF EW(Borders[Right],Borders[Bottom])         ;
  QPointF PS(ls,ls)                                  ;
  PS  = Options -> position ( PS )                   ;
  LW  = Options -> position ( LW )                   ;
  EW  = Options -> position ( EW )                   ;
  ////////////////////////////////////////////////////
  QRectF CR = Composes[page]                         ;
  double xp = CR.left  ()                            ;
  double yp = CR.top   ()                            ;
  double dw = CR.width ()                            ;
  xp        = ps.x()                                 ;
  yp        = ps.y()                                 ;
  dw       -= LW.x()                                 ;
  dw       -= EW.x()                                 ;
  dh       += PS.y()                                 ;
  QSizeF WS(dw,ph)                                   ;
  ////////////////////////////////////////////////////
  textCursor.Block = npid                            ;
  bool request = false                               ;
  int  length  = Document->length(textCursor)        ;
  int  paraAt  = textCursor.Paragraph                ;
  int  sectAt  = 0                                   ;
  textCursor.Paragraph = 0                           ;
  while (!request && textCursor.Paragraph<length)    {
    QPointF       position ( xp       , yp )         ;
    QRectF        RT       ( position , WS )         ;
    TextBlock   * block = (*Document)[textCursor]    ;
    TextSection * TS                                 ;
    if (sectAt<block->count())                       {
      TS = (*block)[sectAt]                          ;
    } else                                           {
      TS = block->NewSection("")                     ;
    }                                                ;
    TS->region     = SectionRect(RT)                 ;
    TS->section    = Document->Measure               (
                        textCursor                   ,
                        Painter.fonts [FONTID]       ,
                        RT                         ) ;
    TS->position   = position                        ;
    setDefaultSection ( *TS )                        ;
    textCursor.Paragraph += TS->length()             ;
    if (sectAt>=block->count())                      {
      block->join(TS)                                ;
      fina = false                                   ;
    }                                                ;
    sectAt ++                                        ;
    yp += dh                                         ;
    if ((yp+dh)>CR.bottom())                         {
      Document -> boundingRect(textCursor)           ;
      page++                                         ;
      if (page<Composes.count())                     {
        CR  = Composes[page]                         ;
        xp  = CR.left  ()                            ;
        yp  = CR.top   ()                            ;
        dw  = CR.width ()                            ;
        dw -= LW.x()                                 ;
        dw -= EW.x()                                 ;
        dh += PS.y()                                 ;
        WS  . setWidth  ( dw )                       ;
        WS  . setHeight ( ph )                       ;
      } else                                         {
        fina    = false                              ;
        request = true                               ;
      }                                              ;
    }                                                ;
  }                                                  ;
  Document->boundingRect(textCursor)                 ;
  QPointF pe = tb->Rectangle().bottomLeft()          ;
  if ((pe.y()-0.1)>pt.y()) fina = false              ;
  ////////////////////////////////////////////////////
  Document -> EnumSections ( )                       ;
  textCursor.Full = textCursor.Section->block        ;
  textCursor.Paragraph = paraAt                      ;
  sectAt = 0                                         ;
  done   = false                                     ;
  while (!done)                                      {
    if (sectAt<tb->count())                          {
      is = (*tb)[sectAt]                             ;
      if (paraAt>is->length())                       {
        paraAt -= is->length()                       ;
        sectAt ++                                    ;
      } else                                         {
        textCursor . Section = is                    ;
        textCursor . Full    = is->block             ;
        textCursor . At      = paraAt                ;
        textCursor . Block   =
                  Document->indexOf(textCursor.Full) ;
        done = true                                  ;
      }                                              ;
    } else done = true                               ;
  }                                                  ;
  ////////////////////////////////////////////////////
  if (request) fina = false                          ;
  locator . unlock ( )                               ;
  return fina                                        ;
}

void N::VcfParagraph::Arrange(void)
{
  nDropOut ( IsNull(Document         ) )             ;
  nDropOut ( !textCursor.isActivated() )             ;
  TextCursor TC ( arrangement )                      ;
  ////////////////////////////////////////////////////
  TextSection * ts    = TC.Section                   ;
  TextBlock   * tb    = ts->block                    ;
  QPointF       ps    = tb->position                 ;
  bool          done  = false                        ;
  bool          fatal = false                        ;
  int           page  = atPage(ps)                   ;
  int           npid  = Document->indexOf(tb)        ;
  nDropOut ( page < 0 )                              ;
  nDropOut ( npid < 0 )                              ;
  locator . lock ( )                                 ;
  ////////////////////////////////////////////////////
  double  ls   = Options->LineSpace                  ;
  double  ph   = Painter.fonts[FONTID].pixelSize()   ;
  double  dh   = ph                                  ;
  QPointF LW(Borders[Left ],Borders[Top   ])         ;
  QPointF EW(Borders[Right],Borders[Bottom])         ;
  QPointF PS(ls,ls)                                  ;
  PS  = Options -> position ( PS )                   ;
  LW  = Options -> position ( LW )                   ;
  EW  = Options -> position ( EW )                   ;
  ////////////////////////////////////////////////////
  QRectF CR = Composes[page]                         ;
  double xp = CR.left  ()                            ;
  double yp = CR.top   ()                            ;
  double dw = CR.width ()                            ;
  xp        = ps.x()                                 ;
  yp        = ps.y()                                 ;
  dw       -= LW.x()                                 ;
  dw       -= EW.x()                                 ;
  dh       += PS.y()                                 ;
  QSizeF WS(dw,ph)                                   ;
  ////////////////////////////////////////////////////
  TC . Page      = page                              ;
  TC . Block     = npid                              ;
  TC . Paragraph = 0                                 ;
  while (!done && (TC.Block<Document->blockCount())) {
    bool keep   = true                               ;
    int  length = Document->length(TC)               ;
    int  At     = 0                                  ;
    tb = (*Document)[TC]                             ;
    if (length>0)                                    {
      if ( At >= tb->count()         ) keep = false  ;
      if ( TC  . Paragraph >= length ) keep = false  ;
      while (!done && keep)                          {
        QPointF       position ( xp       , yp )     ;
        QRectF        RT       ( position , WS )     ;
        TextSection * is                             ;
        if (At>=tb->count())                         {
          is  = tb -> NewSection ( "" )              ;
          tb -> join(is)                             ;
        } else                                       {
          is  = (*tb)[At]                            ;
        }                                            ;
        is->region   = SectionRect(RT)               ;
        is->section  = Document->Measure             (
                         TC                          ,
                         Painter.fonts [FONTID]      ,
                         RT                        ) ;
        is->position = position                      ;
        setDefaultSection ( *is )                    ;
        TC.Paragraph += is->length()                 ;
        At ++                                        ;
        yp += dh                                     ;
        if ((yp+dh)>CR.bottom())                     {
          Document -> boundingRect(TC)               ;
          page++                                     ;
          if (page<Composes.count())                 {
            CR  = Composes[page]                     ;
            xp  = CR.left  ()                        ;
            yp  = CR.top   ()                        ;
            dw  = CR.width ()                        ;
            dw -= LW.x()                             ;
            dw -= EW.x()                             ;
            WS  . setWidth  ( dw )                   ;
            WS  . setHeight ( ph )                   ;
          } else                                     {
            fatal = true                             ;
            done  = true                             ;
          }                                          ;
        }                                            ;
        if ( TC . Paragraph >= length ) keep = false ;
      }                                              ;
      Document->boundingRect(TC)                     ;
    } else                                           {
      QPointF       position ( xp       , yp )       ;
      QRectF        RT       ( position , WS )       ;
      TextSection * is                               ;
      if (tb->count()<=0)                            {
        is  = tb -> NewSection ( "" )                ;
        tb -> join(is)                               ;
      } else                                         {
        is  = (*tb)[At]                              ;
      }                                              ;
      is -> region      = SectionRect(RT)            ;
      is -> section     = ""                         ;
      is -> position    = position                   ;
      setDefaultSection ( *is )                      ;
      Document -> boundingRect(TC)                   ;
      yp += dh                                       ;
      if ((yp+dh)>CR.bottom())                       {
        Document -> boundingRect(TC)                 ;
        page++                                       ;
        if (page<Composes.count())                   {
          CR  = Composes[page]                       ;
          xp  = CR.left  ()                          ;
          yp  = CR.top   ()                          ;
          dw  = CR.width ()                          ;
          dw -= LW.x()                               ;
          dw -= EW.x()                               ;
          WS  . setWidth  ( dw )                     ;
          WS  . setHeight ( ph )                     ;
        } else                                       {
          fatal = true                               ;
          done  = true                               ;
        }                                            ;
      }                                              ;
    }                                                ;
    TC . Block ++                                    ;
    TC . Paragraph = 0                               ;
  }                                                  ;
  ////////////////////////////////////////////////////
  Document -> EnumSections ( )                       ;
  arrangement = TC                                   ;
  LocateCursor ( )                                   ;
  ////////////////////////////////////////////////////
  QRectF S = Composes[0]                             ;
  QPainterPath Path                                  ;
  Path.addRect(S)                                    ;
  for (int i=1;i<Composes.count();i++)               {
    QRectF O = Composes[i]                           ;
    S = S.united(O)                                  ;
    Path.addRect(O)                                  ;
  }                                                  ;
  ScreenRect = S                                     ;
  PaperRect  = Options->Standard(S)                  ;
  ////////////////////////////////////////////////////
  Painter    . pathes [ 1 ] = Path                   ;
  EnablePath  ( 1 , true )                           ;
  MergePathes ( 0        )                           ;
  ////////////////////////////////////////////////////
  locator . unlock ( )                               ;
  if (fatal) emit acquirePaper(this) ; else          {
  }                                                  ;
  prepareGeometryChange           (  )               ;
}

void N::VcfParagraph::EndPartition(void)
{
  nDropOut (Composes.count()<=0)       ;
  nDropOut (IsNull(Document)   )       ;
  textCursor  . empty        ( )       ;
  Document   -> EnumSections ( )       ;
  //////////////////////////////////////
  QRectF S = Composes[0]               ;
  QPainterPath Path                    ;
  Path.addRect(S)                      ;
  for (int i=1;i<Composes.count();i++) {
    QRectF O = Composes[i]             ;
    S = S.united(O)                    ;
    Path.addRect(O)                    ;
  }                                    ;
  ScreenRect = S                       ;
  PaperRect  = Options->Standard(S)    ;
  //////////////////////////////////////
  Painter    . pathes [ 1 ] = Path     ;
  EnablePath  ( 1 , true )             ;
  MergePathes ( 0        )             ;
  prepareGeometryChange           (  ) ;
}

bool N::VcfParagraph::Menu(QPointF pos)
{
  emit ParagraphMenu ( this , pos ) ;
  return true                       ;
}

void N::VcfParagraph::CopyToClipboard(void)
{
  nDropOut ( IsNull(Document)                 )     ;
  nDropOut ( textCursor.Selections.count()<=0 )     ;
  QString text = Document->selectedText(textCursor) ;
  nSetClipboard(text)                               ;
}

void N::VcfParagraph::Paste(void)
{
  nDropOut ( !isCursorEditing() )         ;
  QString    text = nClipboardText        ;
  nDropOut ( text . length () <= 0 )      ;
  QStringList s = text.split('\n')        ;
  if (s.count()<2 && text.length()<90)    {
    if (insertText(text))                 {
      emit contentModified ( this )       ;
      update ( )                          ;
    }                                     ;
  } else                                  {
    if (Document->Paste(textCursor,text)) {
      ArrangeBlock ( )                    ;
      Arrange      ( )                    ;
      LocateCursor ( )                    ;
      LocateAnchor ( )                    ;
    }                                     ;
    emit contentModified ( this )         ;
    update ( )                            ;
  }                                       ;
}

void N::VcfParagraph::Cut(void)
{
  nDropOut ( !isCursorEditing() ) ;
  Document->Cut(textCursor)       ;
  ArrangeBlock ( )                ;
  Arrange      ( )                ;
  LocateCursor ( )                ;
  LocateAnchor ( )                ;
  emit contentModified ( this )   ;
  update       ( )                ;
}

void N::VcfParagraph::SelectAll(void)
{
  nDropOut ( !isCursorEditing() ) ;
  Document->SelectAll(textCursor) ;
  update   (                    ) ;
}

void N::VcfParagraph::ClearSelection(void)
{
  nDropOut     ( !isCursorEditing() ) ;
  LocateAnchor (                    ) ;
  update       (                    ) ;
}

bool N::VcfParagraph::insertText(QString text)
{
  nKickOut ( !isCursorEditing(),false  ) ;
  if (Document->Insert(textCursor,text)) {
    if (!ArrangeBlock())                 {
      Arrange    ( )                     ;
    }                                    ;
    LocateCursor ( )                     ;
    LocateAnchor ( )                     ;
  } else                                 {
  }                                      ;
  return true                            ;
}

bool N::VcfParagraph::insertIM(QString commit,QString preedit)
{
  if (commit.length()>0)            {
    if (!textCursor.InputMethod)    {
      textCursor . EnterIM ( )      ;
    }                               ;
    textCursor . LeaveIM ( )        ;
    Document  -> EnumSections ( )   ;
    if (insertText(commit))         {
      emit contentModified ( this ) ;
      update ( )                    ;
      return true                   ;
    } else return false             ;
  } else
  if (preedit.length()>0)           {
    textCursor . EnterIM ( )        ;
    if (insertText(preedit))        {
      emit contentModified ( this ) ;
      update ( )                    ;
      return true                   ;
    } else return false             ;
  }                                 ;
  return true                       ;
}
