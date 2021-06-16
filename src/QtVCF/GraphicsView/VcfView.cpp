#include <qtvcf.h>
//#include <Photography>

N::VcfView:: VcfView        (QWidget * parent,Plan * p)
           : GraphicsView   (          parent,       p)
           , Mode           (0                        )
           , Method         (0                        )
           , Group          (0                        )
           , Uuid           (0                        )
           , skipMouse      (false                    )
{
  setRelocation ( PerfectRegion ) ;
}

N::VcfView::~VcfView(void)
{
}

void N::VcfView::dragEnterEvent(QDragEnterEvent * event)
{
  QGraphicsView::dragEnterEvent(event);
}

void N::VcfView::dragLeaveEvent(QDragLeaveEvent * event)
{
  QGraphicsView::dragLeaveEvent(event);
}

void N::VcfView::dragMoveEvent(QDragMoveEvent * event)
{
  QGraphicsView::dragMoveEvent(event);
}

void N::VcfView::dropEvent(QDropEvent * event)
{
  QGraphicsView::dropEvent(event);
}

bool N::VcfView::focusNextPrevChild(bool next)
{
  return QGraphicsView::focusNextPrevChild(next);
}

void N::VcfView::focusInEvent(QFocusEvent * event)
{
  if (!focusIn (event)) QGraphicsView::focusInEvent (event) ;
}

void N::VcfView::focusOutEvent(QFocusEvent * event)
{
  if (!focusOut(event)) QGraphicsView::focusOutEvent(event) ;
}

void N::VcfView::inputMethodEvent(QInputMethodEvent * event)
{
  QGraphicsView::inputMethodEvent(event);
}

void N::VcfView::keyPressEvent(QKeyEvent * event)
{
  QGraphicsView::keyPressEvent(event);
}

void N::VcfView::keyReleaseEvent(QKeyEvent * event)
{
  QGraphicsView::keyReleaseEvent(event);
}

void N::VcfView::contextMenuEvent(QContextMenuEvent * event)
{
  QGraphicsView::contextMenuEvent(event);
}

void N::VcfView::mousePressEvent(QMouseEvent * event)
{
  if (skipMouse)                        {
    event->accept()                     ;
    return                              ;
  }                                     ;
  QGraphicsView::mousePressEvent(event) ;
}

void N::VcfView::mouseReleaseEvent(QMouseEvent * event)
{
  if (skipMouse)                          {
    event->accept()                       ;
    return                                ;
  }                                       ;
  QGraphicsView::mouseReleaseEvent(event) ;
}

void N::VcfView::mouseMoveEvent(QMouseEvent * event)
{
  if (skipMouse)                       {
    event->accept()                    ;
    return                             ;
  }                                    ;
  QGraphicsView::mouseMoveEvent(event) ;
}

void N::VcfView::mouseDoubleClickEvent(QMouseEvent * event)
{
  if (skipMouse)                              {
    event->accept()                           ;
    return                                    ;
  }                                           ;
  QGraphicsView::mouseDoubleClickEvent(event) ;
}

void N::VcfView::wheelEvent(QWheelEvent * event)
{
  if (skipMouse)                   {
    event->accept()                ;
    return                         ;
  }                                ;
  QGraphicsView::wheelEvent(event) ;
}

void N::VcfView::resizeEvent(QResizeEvent * event)
{
  if (Relocation()) event->accept (     ) ;
  else QGraphicsView::resizeEvent (event) ;
}

bool N::VcfView::FocusIn(void)
{
  if (IsNull(plan)) return false                  ;
  DisableAllActions (                           ) ;
  AssignAction      ( Label   , windowTitle ( ) ) ;
  LinkAction        ( ZoomIn  , ZoomIn      ( ) ) ;
  LinkAction        ( ZoomOut , ZoomOut     ( ) ) ;
  Alert             ( Click                     ) ;
  return true                                     ;
}

void N::VcfView::PerfectView(void)
{
  View = asPaper (available(size())) ;
  setSceneRect   (View             ) ;
  setTransform   (Transform        ) ;
}

bool N::VcfView::Relocation(void)
{
  if (IsNull(plan)) return false ;
  QWidget * vp = viewport()      ;
  if (IsNull(vp)) return false   ;
  Zoom ()                        ;
  return false                   ;
}

bool N::VcfView::Menu(QPoint pos)
{
  return false ;
}

void N::VcfView::DefaultZooms(void)
{
  setDefaultZoom ( ) ;
}

void N::VcfView::ZoomIn(void)
{
  Enlarge (       ) ;
  Zoom    (       ) ;
  Alert   ( Click ) ;
}

void N::VcfView::ZoomOut(void)
{
  Shrink (       ) ;
  Zoom   (       ) ;
  Alert  ( Click ) ;
}

void N::VcfView::UnitedZoom(void)
{
  QRectF V = asPaper (available(size())) ;
  View     = United  (this,V           ) ;
  setSceneRect       (View             ) ;
  setTransform       (Transform        ) ;
}

void N::VcfView::PaperZoom(void)
{
  QRectF V = asPaper (available(size()))   ;
  if (range.width()>0 && range.height()>0) {
    View = range                           ;
  } else View = V                          ;
  setSceneRect       (View             )   ;
  setTransform       (Transform        )   ;
}

void N::VcfView::AddPaper(void)
{
  VcfPaperManager::AddPaper                              (
    this , Scene , &Options                              ,
    SLOT(PaperMoving(QString,QPointF,QPointF,QPointF)) ) ;
  Zoom()                                                 ;
}

void N::VcfView::Zoom(void)
{
  switch (RelocationMode) {
    case PerfectRegion    :
      PerfectView ()      ;
    break                 ;
    case UnitedRegion     :
      UnitedZoom  ()      ;
    break                 ;
    case PaperRegion      :
      PaperZoom  ()       ;
    break                 ;
  }                       ;
}

N::VcfCanvas * N::VcfView::NewCanvas(int mode,VcfItem * vcf)
{
  VcfCanvas * vc = NewCanvas(vcf) ;
  vc -> Mode = mode               ;
  return vc                       ;
}

N::VcfCanvas * N::VcfView::NewCanvas(VcfItem * vcf)
{
  VcfCanvas * vc = new VcfCanvas(this,vcf,plan)            ;
  addItem ( vc , vcf )                                     ;
  scene()->addItem ( vc )                                  ;
  vc->Options = &Options                                   ;
  connect(vc  ,SIGNAL(Menu          (VcfCanvas*,QPointF))  ,
          this,SLOT  (CanvasMenu    (VcfCanvas*,QPointF))) ;
  connect(vc  ,SIGNAL(ShapeName(VcfCanvas*,QString))       ,
          this,SLOT  (ShapeName(VcfCanvas*,QString))     ) ;
  return vc                                                ;
}

N::VcfPaper * N::VcfView::NewPaper(VcfItem * vcf)
{
  VcfPaper * vp = new VcfPaper(this,vcf,plan) ;
  addItem ( vp , vcf )                        ;
  scene()->addItem ( vp )                        ;
  vp->Options = &Options                      ;
  return vp                                   ;
}

N::VcfPicture * N::VcfView::NewPicture(VcfItem * vcf)
{
  VcfPicture * vp = new VcfPicture(this,vcf,plan)                   ;
  addItem ( vp , vcf )                                              ;
  scene()->addItem ( vp )                                              ;
  vp->Options = &Options                                            ;
  nConnect ( vp  ,SIGNAL(Selection          (VcfItem*,QRectF))      ,
             this,SLOT  (Selection          (VcfItem*,QRectF))    ) ;
  nConnect ( vp  ,SIGNAL(Canvas             (VcfItem*,QRectF))      ,
             this,SLOT  (Canvas             (VcfItem*,QRectF))    ) ;
  nConnect ( vp  ,SIGNAL(Delete             (VcfPicture*))          ,
             this,SLOT  (DeletePicture      (VcfPicture*))        ) ;
  nConnect ( vp  ,SIGNAL(Store              (VcfPicture*))          ,
             this,SLOT  (StorePicture       (VcfPicture*))        ) ;
  nConnect ( vp  ,SIGNAL(Menu               (VcfPicture*,QPointF))  ,
             this,SLOT  (PictureMenu        (VcfPicture*,QPointF))) ;
  nConnect ( vp  ,SIGNAL(Process            (VcfPicture*))          ,
             this,SLOT  (PictureProcess     (VcfPicture*))        ) ;
  nConnect ( vp  ,SIGNAL(Faces              (VcfPicture*))          ,
             this,SLOT  (PictureFaces       (VcfPicture*))        ) ;
  nConnect ( vp  ,SIGNAL(FacesAnalysis      (VcfPicture*))          ,
             this,SLOT  (FacesAnalysis      (VcfPicture*))        ) ;
  nConnect ( vp  ,SIGNAL(LoadSelections     (VcfPicture*))          ,
             this,SLOT  (PictureSelections  (VcfPicture*))        ) ;
  nConnect ( vp  ,SIGNAL(ColorDistribution  (VcfPicture*))          ,
             this,SLOT  (ColorDistribution  (VcfPicture*))        ) ;
  nConnect ( vp  ,SIGNAL(TransformPicture   (VcfPicture*))          ,
             this,SLOT  (TransformPicture   (VcfPicture*))        ) ;
  nConnect ( vp  ,SIGNAL(KeyPoints          (VcfPicture*))          ,
             this,SLOT  (KeyPoints          (VcfPicture*))        ) ;
  nConnect ( vp  ,SIGNAL(Channel            (VcfPicture*,int))      ,
             this,SLOT  (PictureChannel     (VcfPicture*,int))    ) ;
  return vp                                                         ;
}

N::VcfPicture * N::VcfView::NewPicture(VcfItem * vcf,SUID uuid)
{
  VcfPicture * vp = NewPicture(vcf) ;
  vp   -> LoadImage ( uuid )        ;
  return vp                         ;
}

N::VcfPicture * N::VcfView::NewPicture(VcfItem * vcf,QImage image)
{
  VcfPicture * vp = NewPicture(vcf) ;
  vp   -> Image   = image           ;
  return vp                         ;
}

N::VcfLabel * N::VcfView::NewLabel(VcfItem * vcf,QRectF paper,QString content)
{
  VcfLabel * label = new VcfLabel(this,vcf,plan)         ;
  label->Content = content                               ;
  addItem ( label , vcf )                                ;
  scene()->addItem ( label )                             ;
  Font FONT(plan->fonts[Fonts::Default])                 ;
  FONT.cm = 0.48                                         ;
  label->Options = &Options                              ;
  label->Painter.fonts [0] = FONT                        ;
  label->setRange ( paper )                              ;
  connect(label,SIGNAL(Selection     (VcfItem*,QRectF))  ,
          this ,SLOT  (Selection     (VcfItem*,QRectF))) ;
  connect(label,SIGNAL(Canvas        (VcfItem*,QRectF))  ,
          this ,SLOT  (Canvas        (VcfItem*,QRectF))) ;
  return label                                           ;
}

N::VcfColorPicker * N::VcfView::NewColorPicker(VcfItem * vcf)
{
  VcfColorPicker * vcp = new VcfColorPicker(this,NULL,plan) ;
  addItem ( vcp , vcf )                                     ;
  scene()->addItem ( vcp )                                  ;
  nConnect ( vcp  , SIGNAL(RemoveItem(VcfItem*))            ,
             this , SLOT  (Remove    (VcfItem*))          ) ;
  nConnect ( vcp  , SIGNAL(Append(VcfItem*,VcfItem*))       ,
             this , SLOT  (Append(VcfItem*,VcfItem*))     ) ;
  vcp -> Mode    = VcfCanvas::Board                         ;
  vcp -> Options = &Options                                 ;
  return vcp                                                ;
}

N::VcfInterface * N::VcfView::NewInterface(VcfItem * vcf)
{
  VcfInterface * vci = new VcfInterface ( this , NULL , plan ) ;
  addItem ( vci , vcf )                                        ;
  scene   (           ) -> addItem ( vci )                     ;
  vci -> Mode    = VcfCanvas::Board                            ;
  vci -> Options = &Options                                    ;
  return vci                                                   ;
}

void N::VcfView::PutPicture(VcfPicture * picture,double zValue)
{
  picture -> setPos     ( QPointF ( 0.0 , 0.0  ) ) ;
  picture -> setZValue  ( zValue                 ) ;
  picture -> setOpacity ( 1.0                    ) ;
  picture -> setCenter  ( QPointF ( 0.0 , 0.0  ) ) ;
  picture -> setPos     ( picture->PaperMiddle() ) ;
}

void N::VcfView::ShowPicture(VcfPicture * picture)
{
  QRect R = toView(this,picture)                        ;
  setGeometry (QRect(x(),y(),R.width()+8,R.height()+8)) ;
}

void N::VcfView::SolitaryPicture(VcfPicture * picture,double zValue)
{
  Zoom        (                  ) ;
  PutPicture  ( picture , zValue ) ;
  FocusIn     (                  ) ;
  ShowPicture ( picture          ) ;
  Zoom        (                  ) ;
}

bool N::VcfView::validPicture(QFileInfo info)
{
  QImage Image                                           ;
  if (!Image.load(info.absoluteFilePath())) return false ;
  return true                                            ;
}

void N::VcfView::AdjustMdi(QMdiArea * mdi)
{
  QRect V = mapFromScene ( View ) . boundingRect ()                        ;
  QMdiSubWindow * w  = Casting(QMdiSubWindow,parent())                     ;
  QWidget       * vp = viewport()                                          ;
  if (IsNull(w )) return                                                   ;
  if (IsNull(vp)) return                                                   ;
  QScrollBar * sv = verticalScrollBar()                                    ;
  QScrollBar * sh = horizontalScrollBar()                                  ;
  QSize m = mdi->size()                                                    ;
  QSize f = w->geometry().size()                                           ;
  QSize x (V.width(),V.height())                                           ;
  QSize v = vp->size()                                                     ;
  if (NotNull(sv) && sv->isVisible()) v.setWidth (v.width ()+sv->width ()) ;
  if (NotNull(sh) && sh->isVisible()) v.setHeight(v.height()+sh->height()) ;
  f -= v                                                                   ;
  x += f                                                                   ;
  if ( x . width  () > m . width  () )                                     {
    x.setWidth (m.width ())                                                ;
  }                                                                        ;
  if ( x . height () > m . height () )                                     {
    if (NotNull(sv)) x.setWidth (x.width ()+sv->width ())                  ;
    if ( x . width  () > m . width  () ) x.setWidth (m.width ())           ;
    x.setHeight(m.height())                                                ;
  }                                                                        ;
  w->resize(x)                                                             ;
  QPoint tl = w->geometry().topLeft()                                      ;
  x . setWidth  ( x.width () + tl.x() )                                    ;
  x . setHeight ( x.height() + tl.y() )                                    ;
  if ( x . width  () > m . width  () ) tl . setX ( 0 )                     ;
  if ( x . height () > m . height () ) tl . setY ( 0 )                     ;
  w->move(tl)                                                              ;
}

bool N::VcfView::LoadPicture(SUID uuid)
{
  VcfPicture * picture = NewPicture(NULL,uuid) ;
  SolitaryPicture(picture,0.75f);
  QSize S = picture->Image.size() ;
  setWindowTitle(
    tr("%1 - %2 x %3")
      .arg(picture->uuid)
      .arg(S.width ())
      .arg(S.height())
  );
  return true ;
}

bool N::VcfView::LoadPicture(QFileInfo info)
{
  VcfPicture * picture = NewPicture(NULL) ;
  picture->uuid = 0 ;
  picture->Image.load(info.absoluteFilePath()) ;
  SolitaryPicture(picture,0.75f);
  QSize S = picture->Image.size() ;
  setWindowTitle(
    tr("%1 - %2 x %3")
      .arg(info.absoluteFilePath())
      .arg(S.width ())
      .arg(S.height())
  );
  return true ;
}

void N::VcfView::ViewPicture(SUID uuid,MdiArea * Mdi)
{
  setWindowTitle     ( tr("Picture View")    ) ;
  setScene           ( Scene                 ) ;
  setTransform       ( Transform             ) ;
  DefaultZooms       (                       ) ;
  setRelocation      ( UnitedRegion          ) ;
  setScrollBarPolicy ( Qt::ScrollBarAsNeeded ) ;
  LoadPicture        ( uuid                  ) ;
  if (NotNull(Mdi))                            {
    Mdi -> Fit       ( this                  ) ;
  }                                            ;
  show               (                       ) ;
  plan->processEvents(                       ) ;
  if (NotNull(Mdi))                            {
    AdjustMdi        ( Mdi                   ) ;
  }                                            ;
}

void N::VcfView::ViewPicture(QFileInfo info,MdiArea * Mdi)
{
  setWindowTitle     (tr("Picture View")   ) ;
  setScene           (Scene                ) ;
  setTransform       (Transform            ) ;
  DefaultZooms       (                     ) ;
  setRelocation      (UnitedRegion         ) ;
  setScrollBarPolicy (Qt::ScrollBarAsNeeded) ;
  LoadPicture        (info                 ) ;
  if (NotNull(Mdi))                          {
    Mdi -> Fit       (this                 ) ;
  }                                          ;
  show               (                     ) ;
  plan->processEvents(                     ) ;
  if (NotNull(Mdi))                          {
    AdjustMdi        (Mdi                  ) ;
  }                                          ;
}

void N::VcfView::ViewPicture(QImage image,MdiArea * Mdi)
{
  setWindowTitle     (tr("Picture View")   ) ;
  setScene           (Scene                ) ;
  setTransform       (Transform            ) ;
  DefaultZooms       (                     ) ;
  setRelocation      (UnitedRegion         ) ;
  setScrollBarPolicy (Qt::ScrollBarAsNeeded) ;
  ////////////////////////////////////////////
  VcfPicture * picture = NewPicture(NULL)    ;
  picture->uuid = 0                          ;
  picture->Image = image                     ;
  SolitaryPicture(picture,0.75f)             ;
  QSize S = picture->Image.size()            ;
  setWindowTitle                             (
    tr("%2 x %3").arg(S.width ()).arg(S.height()) ) ;
  if (NotNull(Mdi))                          {
    Mdi -> Fit       (this                 ) ;
  }                                          ;
  show               (                     ) ;
  plan->processEvents(                     ) ;
  if (NotNull(Mdi))                          {
    AdjustMdi        (Mdi                  ) ;
  }                                          ;
}

void N::VcfView::DeletePicture(VcfPicture * picture)
{
  Remove ( picture ) ;
}

void N::VcfView::StorePicture(VcfPicture * picture)
{
  emit Store ( Method , Group , picture->uuid ) ;
}

void N::VcfView::PictureMenu(VcfPicture * picture,QPointF pos)
{
  QPoint global = picture -> toGlobal ( pos ) ;
  picture -> showMenu ( this , global )       ;
}

void N::VcfView::PictureProcess(VcfPicture * picture)
{
}

void N::VcfView::PictureFaces(VcfPicture * picture)
{
  PictureManager    PM ( plan )                               ;
  PictureRecognizer PR ( plan )                               ;
  IplImage *        Image                                     ;
  bool              detected = false                          ;
  Image = PM . toIplImage ( picture->Image )                  ;
  if (IsNull(Image)) return                                   ;
  Phiz Faces = PR . Faces ( Image )                           ;
  Alert ( Done )                                              ;
  if (Faces.count()<=0) return                                ;
  QSize  I = picture -> Image . size ()                       ;
  QRectF S = picture -> ScreenRect                            ;
  double Z = picture -> zValue()                              ;
  Z += 0.1 ; if (Z>1.0) Z = 1.0                               ;
  for (int i=0;i<Faces.count();i++)                           {
    QRect   Face                                              ;
    QRectF iFace                                              ;
    Face  = Faces[i].Face                                     ;
    Face  = PR . FlatRatio ( Face , I )                       ;
    iFace = PR . ShiftIn   ( S , Face )                       ;
    VcfCanvas * canvas = NewCanvas ( picture )                ;
    canvas -> Mode = VcfCanvas::Board                         ;
    canvas -> setCanvas  ( iFace      )                       ;
    canvas -> setZValue  ( Z          )                       ;
    canvas -> setOpacity ( 0.35f      )                       ;
    if (Faces[i].isIntact())                                  {
      canvas->Painter.brushes[0].setColor(QColor(192,255,160));
      canvas -> setToolTip                                    (
        tr("Face [%1 x %2]"                                   )
        .arg(Face.width ()                                    )
        .arg(Face.height()                                    )
      )                                                       ;
      detected = true                                         ;
    } else
    if (Faces[i].isPartial())                                 {
      int possibility = (int)( Faces[i].Possibility() * 100 ) ;
      QString msg = tr("Possible %1% face [%2 x %3]"          )
      .arg(possibility                                        )
      .arg(Face.width ()                                      )
      .arg(Face.height()                                    ) ;
      if (Faces[i].LeftEye .count()>0)                        {
        msg += "\n"                                           ;
        msg += tr("Possible left eye detected")               ;
      }                                                       ;
      if (Faces[i].RightEye.count()>0)                        {
        msg += "\n"                                           ;
        msg += tr("Possible right eye detected")              ;
      }                                                       ;
      if (Faces[i].Mouth   .count()>0)                        {
        msg += "\n"                                           ;
        msg += tr("Possible mouth detected")                  ;
      }                                                       ;
      if (Faces[i].Noses   .count()>0)                        {
        msg += "\n"                                           ;
        msg += tr("Possible nose detected")                   ;
      }                                                       ;
      canvas->Painter.brushes[0].setColor(QColor(255,160,192));
      canvas -> setToolTip ( msg )                            ;
      detected = true                                         ;
    }                                                         ;
  }                                                           ;
  if (detected) Alert ( Done  )                               ;
           else Alert ( Error )                               ;
}

void N::VcfView::FacesAnalysis(VcfPicture * picture)
{
  PictureManager    PM ( plan )                               ;
  PictureRecognizer PR ( plan )                               ;
  IplImage *        Image                                     ;
  bool              detected = false                          ;
  Image = PM . toIplImage ( picture->Image )                  ;
  if (IsNull(Image)) return                                   ;
  Phiz Faces = PR . Faces ( Image )                           ;
  Alert ( Done )                                              ;
  if (Faces.count()<=0) return                                ;
  QSize  I = picture -> Image . size ()                       ;
  QRectF S = picture -> ScreenRect                            ;
  double Z = picture -> zValue()                              ;
  Z += 0.1 ; if (Z>1.0) Z = 1.0                               ;
  for (int i=0;i<Faces.count();i++)                           {
    QRect   Face                                              ;
    QRectF iFace                                              ;
    Face  = Faces[i].Face                                     ;
    Face  = PR . FlatRatio ( Face , I )                       ;
    iFace = PR . ShiftIn   ( S , Face )                       ;
    VcfCanvas * canvas = NewCanvas ( picture )                ;
    canvas -> Mode = VcfCanvas::Board                         ;
    canvas -> setCanvas  ( iFace      )                       ;
    canvas -> setZValue  ( Z          )                       ;
    canvas -> setOpacity ( 0.35f      )                       ;
    if (Faces[i].isIntact())                                  {
      canvas->Painter.brushes[0].setColor(QColor(192,255,160));
      canvas -> setToolTip                                    (
        tr("Face [%1 x %2]"                                   )
        .arg(Face.width ()                                    )
        .arg(Face.height()                                    )
      )                                                       ;
      detected = true                                         ;
    } else
    if (Faces[i].isPartial())                                 {
      int possibility = (int)( Faces[i].Possibility() * 100 ) ;
      QString msg = tr("Possible %1% face [%2 x %3]"          )
      .arg(possibility                                        )
      .arg(Face.width ()                                      )
      .arg(Face.height()                                    ) ;
      if (Faces[i].LeftEye .count()>0)                        {
        msg += "\n"                                           ;
        msg += tr("Possible left eye detected")               ;
      }                                                       ;
      if (Faces[i].RightEye.count()>0)                        {
        msg += "\n"                                           ;
        msg += tr("Possible right eye detected")              ;
      }                                                       ;
      if (Faces[i].Mouth   .count()>0)                        {
        msg += "\n"                                           ;
        msg += tr("Possible mouth detected")                  ;
      }                                                       ;
      if (Faces[i].Noses   .count()>0)                        {
        msg += "\n"                                           ;
        msg += tr("Possible nose detected")                   ;
      }                                                       ;
      canvas->Painter.brushes[0].setColor(QColor(255,160,192));
      canvas -> setToolTip ( msg )                            ;
      detected = true                                         ;
    }                                                         ;
    QList<QRect> FS                                           ;
    FS.clear()                                                ;
    if (Faces[i].LeftEye .count()>0) FS << Faces[i].LeftEye   ;
    if (Faces[i].RightEye.count()>0) FS << Faces[i].RightEye  ;
    if (Faces[i].Mouth   .count()>0) FS << Faces[i].Mouth     ;
    if (Faces[i].Noses   .count()>0) FS << Faces[i].Noses     ;
    if (Faces[i].LeftEar .count()>0) FS << Faces[i].LeftEar   ;
    if (Faces[i].RightEar.count()>0) FS << Faces[i].RightEar  ;
    if (Faces[i].Glass.count()>0) FS << Faces[i].Glass        ;
    if (Faces[i].Body.count()>0) FS << Faces[i].Body          ;
    if (Faces[i].UpperBody.count()>0) FS << Faces[i].UpperBody;
    if (Faces[i].LowerBody.count()>0) FS << Faces[i].LowerBody;
    QColor WHITE = QColor(240,240,240)                        ;
    for (int j=0;j<FS.count();j++)                            {
      Face  = FS[j]                                           ;
      iFace = PR . ShiftIn ( S , Face )                       ;
      VcfCanvas * canvas = NewCanvas ( picture )              ;
      canvas -> Mode = VcfCanvas::Board                       ;
      canvas -> setCanvas  ( iFace      )                     ;
      canvas -> setZValue  ( Z          )                     ;
      canvas -> setOpacity ( 0.35f      )                     ;
      canvas -> Painter.brushes[0].setColor(WHITE)            ;
    }                                                         ;
  }                                                           ;
  if (detected) Alert ( Done  )                               ;
           else Alert ( Error )                               ;
}

void N::VcfView::PictureSelections(VcfPicture * picture)
{
  if (IsNull(picture)) return                         ;
  SUID uuid = picture->uuid                           ;
  if (uuid<=0) return                                 ;
  QMap<SUID,QRect> Faces                              ;
  UUIDs            Uuids                              ;
  Sql sql = plan->sql                                 ;
  SqlConnection SC(sql)                               ;
  if (SC.open("VcfGraphicsView","PictureSelections")) {
    QString Q                                         ;
    SUID    xuid                                      ;
    Q = sql.SelectFrom                                (
          "second"                                    ,
          plan->Tables[Tables::Groups]      ,
          QString("where first = %1 and t1 = %2 and t2 = %3 and relation = %4 order by position asc")
            .arg(uuid                                 )
            .arg(Types::Picture             )
            .arg(Types::Rectangle           )
            .arg(Groups::Subordination      )
        )                                             ;
    SqlLoopNow ( SC , Q )                             ;
      xuid = SC . Uuid ( 0 )                          ;
      Uuids << xuid                                   ;
    SqlLoopErr ( SC , Q )                             ;
    SqlLoopEnd ( SC , Q )                             ;
    foreach (xuid,Uuids)                              {
      Q = sql.SelectFrom                              (
            "x,y,w,h"                                 ,
            plan->Tables[Tables::Rectangles],
            QString("where uuid = %1").arg(xuid)    ) ;
      if (SC.Fetch(Q))                                {
        double x = SC . Value ( 0 ) . toDouble ( )    ;
        double y = SC . Value ( 1 ) . toDouble ( )    ;
        double w = SC . Value ( 2 ) . toDouble ( )    ;
        double h = SC . Value ( 3 ) . toDouble ( )    ;
        QRect  R((int)x,(int)y,(int)w,(int)h)         ;
        Faces[xuid] = R                               ;
      }                                               ;
    }                                                 ;
    SC.close()                                        ;
  }                                                   ;
  SC.remove()                                         ;
  if (Faces.count()<=0)                               {
    Alert ( Error )                                   ;
    return                                            ;
  }                                                   ;
  PictureRecognizer PR ( plan )                       ;
  QRectF S = picture -> ScreenRect                    ;
  double Z = picture -> zValue()                      ;
  Z += 0.1 ; if (Z>1.0) Z = 1.0                       ;
  for (int i=0;i<Uuids.count();i++)                   {
    SUID xuid = Uuids[i]                              ;
    QRectF Face = PR . ShiftIn ( S , Faces[xuid] )    ;
    VcfCanvas * canvas = NewCanvas ( picture )        ;
    canvas -> uuid = xuid                             ;
    canvas -> Mode = VcfCanvas::Board                 ;
    canvas -> setCanvas  ( Face  )                    ;
    canvas -> setZValue  ( Z     )                    ;
    canvas -> setOpacity ( 0.35f )                    ;
  }                                                   ;
  Alert ( Done )                                      ;
}

void N::VcfView::ColorDistribution(VcfPicture * picture)
{
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  if (IsNull(picture)) return                      ;
  SUID uuid = picture->uuid                        ;
  if (uuid<=0) return                              ;
  PictureManager PM(plan)                          ;
  QByteArray c = PM.toColors(picture->Image)       ;
  //////////////////////////////////////////////////
  VcfInterface * vic = NewInterface(NULL)          ;
  //////////////////////////////////////////////////
  ViewCommon * vc = new ViewCommon(this,Plan)      ;
  vc->setBackground(QColor(0,0,0))                 ;
  //////////////////////////////////////////////////
  nUniverse  * U = vc->addColorDistribution(0,c  ) ;
  nCameraBox * T = vc->addCameraControl    (1,128) ;
  T   -> assignCamera ( 1 , U->CurrentCamera )     ;
  //////////////////////////////////////////////////
  QRectF  r = picture->boundingRect()              ;
  QPointF p = r.topRight()                         ;
  p = picture->mapToScene(p)                       ;
  p = Options.Standard(p)                          ;
  QRect   W(0, 0,520,540)                          ;
  QRect   C(4,24,512,512)                          ;
  vic -> setChild ( vc , p , W , C )               ;
  //////////////////////////////////////////////////
  vc -> show         (                      )      ;
  vc -> startup      (                      )      ;
  //////////////////////////////////////////////////
#endif
  Alert ( Done )                                   ;
}

void N::VcfView::TransformPicture(VcfPicture  * picture)
{
  if (IsNull(picture)) return                   ;
  SUID uuid = picture->uuid                     ;
  if (uuid<=0) return                           ;
  ///////////////////////////////////////////////
  double     Angle = picture->Angle             ;
  QImage     I     = picture->Image             ;
  QTransform T                                  ;
  T . reset  (       )                          ;
  T . rotate ( Angle )                          ;
  I = I.transformed(T,Qt::SmoothTransformation) ;
  ///////////////////////////////////////////////
  VcfPicture * vp = NewPicture(NULL,I)          ;
  PutPicture ( vp , 0.75f )                     ;
  vp->show()                                    ;
  ///////////////////////////////////////////////
  Alert ( Done )                                ;
}

void N::VcfView::PictureChannel(VcfPicture * picture,int Component)
{
}

N::VcfPicture * N::VcfView::CropPicture(VcfPicture * picture,VcfCanvas * canvas)
{
  QSize   is = picture->Image.size() ;
  QRectF  sr = canvas->mapToItem(picture,canvas->ScreenRect).boundingRect() ;
  QRectF  si = picture->ScreenRect ;
  QRect   sf (sr.left()-si.left(),sr.top()-si.top(),sr.width(),sr.height()) ;
  QRect   fr (0,0,is.width(),is.height()) ;
  QRect   ir = fr.intersected(sf) ;
  QImage  ii = picture->Image.copy(ir) ;
  QRectF  cr ( ir.left()+si.left(),ir.top()+si.top(),ir.width(),ir.height()) ;
  QRectF  st = picture->mapToScene(cr).boundingRect() ;
  QRectF  pr = Options.Standard(st) ;
  QPointF cp = pr.center() ;
  double  Z  = picture->zValue() + 0.20 ;
  if (Z>1.0) Z = 1.0 ;
  VcfPicture * pic = NewPicture(NULL) ;
  pic->uuid = 0 ;
  pic->Image = ii ;
  pic -> setPos     ( QPointF ( 0.0 , 0.0  ) ) ;
  pic -> setZValue  ( Z                      ) ;
  pic -> setOpacity ( 1.0                    ) ;
  pic -> setCenter  ( cp                     ) ;
  return pic ;
}

N::VcfPicture * N::VcfView::SobelPicture(VcfPicture * picture,VcfCanvas * canvas)
{
  QSize   is = picture->Image.size()                                         ;
  QRectF  sr = canvas->mapToItem(picture,canvas->ScreenRect).boundingRect()  ;
  QRectF  si = picture->ScreenRect                                           ;
  QRect   sf (sr.left()-si.left(),sr.top()-si.top(),sr.width(),sr.height())  ;
  QRect   fr (0,0,is.width(),is.height())                                    ;
  QRect   ir = fr.intersected(sf)                                            ;
  QImage  ii = picture->Image.copy(ir)                                       ;
  QRectF  cr ( ir.left()+si.left(),ir.top()+si.top(),ir.width(),ir.height()) ;
  QRectF  st = picture->mapToScene(cr).boundingRect()                        ;
  QRectF  pr = Options.Standard(st)                                          ;
  QPointF cp = pr.center()                                                   ;
  double  Z  = picture->zValue() + 0.20                                      ;
  if (Z>1.0) Z = 1.0                                                         ;
  ii = ii.convertToFormat(QImage::Format_ARGB32)                             ;
  ////////////////////////////////////////////////////////////////////////////
  PictureManager PM(plan)                                                    ;
  QImage   * II      = NULL                                                  ;
  IplImage * src     = PM . toIplImage ( ii   )                              ;
  IplImage * grey    = PM . toGrey     ( src  )                              ;
  IplImage * smooth  = PM . Median     ( grey )                              ;
  IplImage * erode   = PM . Erode      ( smooth )                            ;
  IplImage * dilate  = PM . Dilate     ( erode  )                            ;
  ::cvSobel ( dilate , grey , 1 , 1 , 5 )                                    ;
  II = PM.toImage ( grey )                                                   ;
  if (NotNull(II)) ii = *II                                                  ;
  if (NotNull(II)) delete II                                                 ;
  PM . Release ( src    )                                                    ;
  PM . Release ( grey   )                                                    ;
  PM . Release ( smooth )                                                    ;
  PM . Release ( erode  )                                                    ;
  PM . Release ( dilate )                                                    ;
  ////////////////////////////////////////////////////////////////////////////
  VcfPicture * pic = NewPicture(NULL)                                        ;
  pic -> uuid  = 0                                                           ;
  pic -> Image = ii                                                          ;
  pic -> setPos     ( QPointF ( 0.0 , 0.0  ) )                               ;
  pic -> setZValue  ( Z                      )                               ;
  pic -> setOpacity ( 1.0                    )                               ;
  pic -> setCenter  ( cp                     )                               ;
  ////////////////////////////////////////////////////////////////////////////
  return pic                                                                 ;
}

N::VcfPicture * N::VcfView::VerticalSobel(VcfPicture * picture,VcfCanvas * canvas)
{
  QSize   is = picture->Image.size()                                         ;
  QRectF  sr = canvas->mapToItem(picture,canvas->ScreenRect).boundingRect()  ;
  QRectF  si = picture->ScreenRect                                           ;
  QRect   sf (sr.left()-si.left(),sr.top()-si.top(),sr.width(),sr.height())  ;
  QRect   fr (0,0,is.width(),is.height())                                    ;
  QRect   ir = fr.intersected(sf)                                            ;
  QImage  ii = picture->Image.copy(ir)                                       ;
  QRectF  cr ( ir.left()+si.left(),ir.top()+si.top(),ir.width(),ir.height()) ;
  QRectF  st = picture->mapToScene(cr).boundingRect()                        ;
  QRectF  pr = Options.Standard(st)                                          ;
  QPointF cp = pr.center()                                                   ;
  double  Z  = picture->zValue() + 0.20                                      ;
  if (Z>1.0) Z = 1.0                                                         ;
  ii = ii.convertToFormat(QImage::Format_ARGB32)                             ;
  ////////////////////////////////////////////////////////////////////////////
  PictureManager PM ( plan )                                                 ;
  QImage ri                                                                  ;
  if (!PM.VerticalSobel(ii,ri,32)) return NULL                               ;
  ii = ri                                                                    ;
  ////////////////////////////////////////////////////////////////////////////
  VcfPicture * pic = NewPicture(NULL)                                        ;
  pic -> uuid  = 0                                                           ;
  pic -> Image = ii                                                          ;
  pic -> setPos     ( QPointF ( 0.0 , 0.0  ) )                               ;
  pic -> setZValue  ( Z                      )                               ;
  pic -> setOpacity ( 1.0                    )                               ;
  pic -> setCenter  ( cp                     )                               ;
  ////////////////////////////////////////////////////////////////////////////
  return pic                                                                 ;
}

N::VcfPicture * N::VcfView::HorizontalSobel(VcfPicture * picture,VcfCanvas * canvas)
{
  QSize   is = picture->Image.size()                                         ;
  QRectF  sr = canvas->mapToItem(picture,canvas->ScreenRect).boundingRect()  ;
  QRectF  si = picture->ScreenRect                                           ;
  QRect   sf (sr.left()-si.left(),sr.top()-si.top(),sr.width(),sr.height())  ;
  QRect   fr (0,0,is.width(),is.height())                                    ;
  QRect   ir = fr.intersected(sf)                                            ;
  QImage  ii = picture->Image.copy(ir)                                       ;
  QRectF  cr ( ir.left()+si.left(),ir.top()+si.top(),ir.width(),ir.height()) ;
  QRectF  st = picture->mapToScene(cr).boundingRect()                        ;
  QRectF  pr = Options.Standard(st)                                          ;
  QPointF cp = pr.center()                                                   ;
  double  Z  = picture->zValue() + 0.20                                      ;
  if (Z>1.0) Z = 1.0                                                         ;
  ii = ii.convertToFormat(QImage::Format_ARGB32)                             ;
  ////////////////////////////////////////////////////////////////////////////
  PictureManager PM ( plan )                                                 ;
  QImage ri                                                                  ;
  if (!PM.HorizontalSobel(ii,ri,32)) return NULL                             ;
  ii = ri                                                                    ;
  ////////////////////////////////////////////////////////////////////////////
  VcfPicture * pic = NewPicture(NULL)                                        ;
  pic -> uuid  = 0                                                           ;
  pic -> Image = ii                                                          ;
  pic -> setPos     ( QPointF ( 0.0 , 0.0  ) )                               ;
  pic -> setZValue  ( Z                      )                               ;
  pic -> setOpacity ( 1.0                    )                               ;
  pic -> setCenter  ( cp                     )                               ;
  ////////////////////////////////////////////////////////////////////////////
  return pic                                                                 ;
}

N::VcfPicture * N::VcfView::CannyPicture(VcfPicture * picture,VcfCanvas * canvas)
{
  QSize   is = picture->Image.size()                                         ;
  QRectF  sr = canvas->mapToItem(picture,canvas->ScreenRect).boundingRect()  ;
  QRectF  si = picture->ScreenRect                                           ;
  QRect   sf (sr.left()-si.left(),sr.top()-si.top(),sr.width(),sr.height())  ;
  QRect   fr (0,0,is.width(),is.height())                                    ;
  QRect   ir = fr.intersected(sf)                                            ;
  QImage  ii = picture->Image.copy(ir)                                       ;
  QRectF  cr ( ir.left()+si.left(),ir.top()+si.top(),ir.width(),ir.height()) ;
  QRectF  st = picture->mapToScene(cr).boundingRect()                        ;
  QRectF  pr = Options.Standard(st)                                          ;
  QPointF cp = pr.center()                                                   ;
  double  Z  = picture->zValue() + 0.20                                      ;
  if (Z>1.0) Z = 1.0                                                         ;
  ii = ii.convertToFormat(QImage::Format_ARGB32)                             ;
  ////////////////////////////////////////////////////////////////////////////
  PictureManager PM(plan)                                                    ;
  QImage   * II      = NULL                                                  ;
  IplImage * src     = PM . toIplImage ( ii   )                              ;
  IplImage * grey    = PM . toGrey     ( src  )                              ;
  IplImage * smooth  = PM . Median     ( grey )                              ;
  IplImage * erode   = PM . Erode      ( smooth )                            ;
  IplImage * dilate  = PM . Dilate     ( erode  )                            ;
  ::cvCanny (dilate , grey , 2 , 20 , 3)                                     ;
  II = PM.toImage ( grey )                                                   ;
  if (NotNull(II)) ii = *II                                                  ;
  if (NotNull(II)) delete II                                                 ;
  PM . Release ( src    )                                                    ;
  PM . Release ( grey   )                                                    ;
  PM . Release ( smooth )                                                    ;
  PM . Release ( erode  )                                                    ;
  PM . Release ( dilate )                                                    ;
  ////////////////////////////////////////////////////////////////////////////
  VcfPicture * pic = NewPicture(NULL)                                        ;
  pic -> uuid  = 0                                                           ;
  pic -> Image = ii                                                          ;
  pic -> setPos     ( QPointF ( 0.0 , 0.0  ) )                               ;
  pic -> setZValue  ( Z                      )                               ;
  pic -> setOpacity ( 1.0                    )                               ;
  pic -> setCenter  ( cp                     )                               ;
  ////////////////////////////////////////////////////////////////////////////
  return pic                                                                 ;
}

N::VcfPicture * N::VcfView::LaplacePicture(VcfPicture * picture,VcfCanvas * canvas)
{
  QSize   is = picture->Image.size()                                         ;
  QRectF  sr = canvas->mapToItem(picture,canvas->ScreenRect).boundingRect()  ;
  QRectF  si = picture->ScreenRect                                           ;
  QRect   sf (sr.left()-si.left(),sr.top()-si.top(),sr.width(),sr.height())  ;
  QRect   fr (0,0,is.width(),is.height())                                    ;
  QRect   ir = fr.intersected(sf)                                            ;
  QImage  ii = picture->Image.copy(ir)                                       ;
  QRectF  cr ( ir.left()+si.left(),ir.top()+si.top(),ir.width(),ir.height()) ;
  QRectF  st = picture->mapToScene(cr).boundingRect()                        ;
  QRectF  pr = Options.Standard(st)                                          ;
  QPointF cp = pr.center()                                                   ;
  double  Z  = picture->zValue() + 0.20                                      ;
  if (Z>1.0) Z = 1.0                                                         ;
  ii = ii.convertToFormat(QImage::Format_ARGB32)                             ;
  ////////////////////////////////////////////////////////////////////////////
  PictureManager PM(plan)                                                    ;
  QImage   * II      = NULL                                                  ;
  IplImage * src     = PM . toIplImage ( ii     )                            ;
  IplImage * grey    = PM . toGrey     ( src    )                            ;
  IplImage * smooth  = PM . Median     ( grey   )                            ;
  IplImage * erode   = PM . Erode      ( smooth )                            ;
  IplImage * dilate  = PM . Dilate     ( erode  )                            ;
  IplImage * laplace = PM . Laplace    ( dilate )                            ;
  ::cvConvertScale( laplace , grey )                                         ;
  II = PM.toImage ( grey )                                                   ;
  if (NotNull(II)) ii = *II                                                  ;
  if (NotNull(II)) delete II                                                 ;
  PM . Release ( src     )                                                   ;
  PM . Release ( grey    )                                                   ;
  PM . Release ( smooth  )                                                   ;
  PM . Release ( erode   )                                                   ;
  PM . Release ( dilate  )                                                   ;
  PM . Release ( laplace )                                                   ;
  ////////////////////////////////////////////////////////////////////////////
  VcfPicture * pic = NewPicture(NULL)                                        ;
  pic -> uuid  = 0                                                           ;
  pic -> Image = ii                                                          ;
  pic -> setPos     ( QPointF ( 0.0 , 0.0  ) )                               ;
  pic -> setZValue  ( Z                      )                               ;
  pic -> setOpacity ( 1.0                    )                               ;
  pic -> setCenter  ( cp                     )                               ;
  ////////////////////////////////////////////////////////////////////////////
  return pic                                                                 ;
}

QList<N::VcfContours *> N::VcfView::CannyDetection(VcfPicture * picture,VcfCanvas * canvas)
{
  QList<VcfContours *> contours                                              ;
  ////////////////////////////////////////////////////////////////////////////
  QSize   is = picture->Image.size()                                         ;
  QRectF  sr = canvas->mapToItem(picture,canvas->ScreenRect).boundingRect()  ;
  QRectF  si = picture->ScreenRect                                           ;
  QRect   sf (sr.left()-si.left(),sr.top()-si.top(),sr.width(),sr.height())  ;
  QRect   fr (0,0,is.width(),is.height())                                    ;
  QRect   ir = fr.intersected(sf)                                            ;
  QImage  ii = picture->Image.copy(ir)                                       ;
  ii = ii.convertToFormat(QImage::Format_ARGB32)                             ;
  ////////////////////////////////////////////////////////////////////////////
  PictureManager PM(plan)                                                    ;
  IplImage     * src     = PM . toIplImage ( ii   )                          ;
  IplImage     * grey    = PM . toGrey     ( src  )                          ;
  IplImage     * smooth  = PM . Median     ( grey )                          ;
  IplImage     * erode   = PM . Erode      ( smooth )                        ;
  IplImage     * dilate  = PM . Dilate     ( erode  )                        ;
  CvMemStorage * storage = cvCreateMemStorage(0);                            ;
  CvSeq        * contour = NULL                                              ;
  ::cvCanny (dilate , grey , 3 , 15 , 3)                                     ;
  ////////////////////////////////////////////////////////////////////////////
  ::cvFindContours(grey,storage,&contour,sizeof(CvContour)                   ,
//    CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE                                ) ;
//    CV_RETR_LIST    ,CV_CHAIN_APPROX_NONE                                  ) ;
//    CV_RETR_CCOMP   ,CV_CHAIN_APPROX_NONE                                  ) ;
    CV_RETR_TREE    ,CV_CHAIN_APPROX_SIMPLE                                ) ;
  if (NotNull(contour))                                                      {
    QPointF RC (sf.left(),sf.top())                                          ;
    QPointF PC   = picture->ScreenRect.topLeft ( )                           ;
    PC += RC                                                                 ;
    contour = ::cvApproxPoly(contour,sizeof(CvContour),storage               ,
                CV_POLY_APPROX_DP,3,1                                      ) ;
    CvTreeNodeIterator iterator                                              ;
    ::cvInitTreeNodeIterator ( &iterator , contour , 3                     ) ;
    CvSeq * allpointsSeq = ::cvCreateSeq                                     (
                             CV_SEQ_KIND_GENERIC | CV_32SC2                  ,
                             sizeof(CvContour),sizeof(CvPoint),storage     ) ;
    while ( NULL != ( contour = (CvSeq *)cvNextTreeNode(&iterator) ) )       {
      VcfContours * vct = new VcfContours(this,canvas)                       ;
      connect(vct ,SIGNAL(Menu       (VcfContours*,QPointF))                 ,
              this,SLOT  (ContourMenu(VcfContours*,QPointF))               ) ;
      addItem ( vct , canvas )                                               ;
      scene()->addItem ( vct )                                                  ;
      vct->plan    = plan                                                    ;
      vct->Options = &Options                                                ;
      int len = contour->total                                               ;
      CvSeqReader reader                                                     ;
      CvPoint     pt                                                         ;
      ::cvStartReadSeq ( contour , &reader )                                 ;
      vct->contour . type = Graphics::Linear                                 ;
      vct->contour . begin()                                                 ;
      vct->Painter . pens [ 1 ] . setColor ( QColor(  0,  0,255) )           ;
      for (int i=0;i<len;i++)                                                {
        CV_READ_SEQ_ELEM(pt,reader)                                          ;
        QPointF P ( pt.x , pt.y )                                            ;
        P += PC                                                              ;
        P  = picture -> mapToItem ( canvas , P )                             ;
        P  = Options  . Standard  ( P          )                             ;
        ControlPoint ncp                                                     ;
        ncp.Type  = Graphics::Linear                                         ;
        ncp.Type |= Graphics::Flat                                           ;
        ncp.x     = P . x ( )                                                ;
        ncp.y     = P . y ( )                                                ;
        vct -> contour . add ( i , ncp )                                     ;
        ::cvSeqPush(allpointsSeq,&pt)                                        ;
      }                                                                      ;
      vct->contour.close(Graphics::Flat)                                     ;
      vct->Prepare()                                                         ;
      contours << vct                                                        ;
    }                                                                        ;
  }                                                                          ;
//  CvSeq * hull = ::cvConvexHull2 ( allpointsSeq , 0 , CV_CLOCKWISE , 0 )     ;
  ////////////////////////////////////////////////////////////////////////////
  PM . Release ( src    )                                                    ;
  PM . Release ( grey   )                                                    ;
  PM . Release ( smooth )                                                    ;
  PM . Release ( erode  )                                                    ;
  PM . Release ( dilate )                                                    ;
  ::cvClearMemStorage   (  storage )                                         ;
  ::cvReleaseMemStorage ( &storage )                                         ;
  ////////////////////////////////////////////////////////////////////////////
  return contours                                                            ;
}

QList<N::VcfContours *> N::VcfView::SobelDetection(VcfPicture * picture,VcfCanvas * canvas)
{
  QList<VcfContours *> contours                                              ;
  ////////////////////////////////////////////////////////////////////////////
  QSize   is = picture->Image.size()                                         ;
  QRectF  sr = canvas->mapToItem(picture,canvas->ScreenRect).boundingRect()  ;
  QRectF  si = picture->ScreenRect                                           ;
  QRect   sf (sr.left()-si.left(),sr.top()-si.top(),sr.width(),sr.height())  ;
  QRect   fr (0,0,is.width(),is.height())                                    ;
  QRect   ir = fr.intersected(sf)                                            ;
  QImage  ii = picture->Image.copy(ir)                                       ;
  ii = ii.convertToFormat(QImage::Format_ARGB32)                             ;
  ////////////////////////////////////////////////////////////////////////////
  PictureManager PM(plan)                                                    ;
  IplImage * src     = PM . toIplImage ( ii   )                              ;
  IplImage * grey    = PM . toGrey     ( src  )                              ;
  IplImage * smooth  = PM . Median     ( grey )                              ;
  IplImage * erode   = PM . Erode      ( smooth )                            ;
  IplImage * dilate  = PM . Dilate     ( erode  )                            ;
  IplImage * laplace = PM . Laplace    ( dilate )                            ;
  ::cvConvertScale( laplace , grey )                                         ;
  CvMemStorage * storage = cvCreateMemStorage(0);                            ;
  CvSeq        * contour = NULL                                              ;
  ////////////////////////////////////////////////////////////////////////////
  ::cvFindContours(grey,storage,&contour,sizeof(CvContour)                   ,
//    CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE                                  ) ;
//    CV_RETR_LIST    ,CV_CHAIN_APPROX_NONE                                  ) ;
//    CV_RETR_CCOMP   ,CV_CHAIN_APPROX_NONE                                  ) ;
    CV_RETR_TREE    ,CV_CHAIN_APPROX_SIMPLE                                ) ;
  if (NotNull(contour))                                                      {
    QPointF RC (sf.left(),sf.top())                                          ;
    QPointF PC   = picture->ScreenRect.topLeft ( )                           ;
    PC += RC                                                                 ;
    contour = ::cvApproxPoly(contour,sizeof(CvContour),storage               ,
                CV_POLY_APPROX_DP,3,1                                      ) ;
    CvTreeNodeIterator iterator                                              ;
    ::cvInitTreeNodeIterator ( &iterator , contour , 3                     ) ;
    CvSeq * allpointsSeq = ::cvCreateSeq                                     (
                             CV_SEQ_KIND_GENERIC | CV_32SC2                  ,
                             sizeof(CvContour),sizeof(CvPoint),storage     ) ;
    while ( NULL != ( contour = (CvSeq *)cvNextTreeNode(&iterator) ) )       {
      VcfContours * vct = new VcfContours(this,canvas)                       ;
      connect(vct ,SIGNAL(Menu       (VcfContours*,QPointF))                 ,
              this,SLOT  (ContourMenu(VcfContours*,QPointF))               ) ;
      addItem ( vct , canvas )                                               ;
      scene()->addItem ( vct )                                                  ;
      vct->plan    = plan                                                    ;
      vct->Options = &Options                                                ;
      int len = contour->total                                               ;
      CvSeqReader reader                                                     ;
      CvPoint     pt                                                         ;
      ::cvStartReadSeq ( contour , &reader )                                 ;
      vct->contour . type = Graphics::Linear                         ;
      vct->contour . begin()                                                 ;
      vct->Painter . pens [ 1 ] . setColor ( QColor(  0,  0,255) )           ;
      for (int i=0;i<len;i++)                                                {
        CV_READ_SEQ_ELEM(pt,reader)                                          ;
        QPointF P ( pt.x , pt.y )                                            ;
        P += PC                                                              ;
        P  = picture -> mapToItem ( canvas , P )                             ;
        P  = Options  . Standard  ( P          )                             ;
        ControlPoint ncp                                                     ;
        ncp.Type  = Graphics::Linear                                         ;
        ncp.Type |= Graphics::Flat                                           ;
        ncp.x     = P . x ( )                                                ;
        ncp.y     = P . y ( )                                                ;
        vct -> contour . add ( i , ncp )                                     ;
        ::cvSeqPush(allpointsSeq,&pt)                                        ;
      }                                                                      ;
      vct->contour.close(Graphics::Flat)                                     ;
      vct->Prepare()                                                         ;
      contours << vct                                                        ;
    }                                                                        ;
  }                                                                          ;
//  CvSeq * hull = ::cvConvexHull2 ( allpointsSeq , 0 , CV_CLOCKWISE , 0 )     ;
  ////////////////////////////////////////////////////////////////////////////
  PM . Release ( src    )                                                    ;
  PM . Release ( grey   )                                                    ;
  PM . Release ( smooth )                                                    ;
  PM . Release ( erode  )                                                    ;
  PM . Release ( dilate )                                                    ;
  ::cvClearMemStorage   (  storage )                                         ;
  ::cvReleaseMemStorage ( &storage )                                         ;
  ////////////////////////////////////////////////////////////////////////////
  return contours                                                            ;
}

QList<N::VcfContours *> N::VcfView::LaplaceDetection(VcfPicture * picture,VcfCanvas * canvas)
{
  QList<VcfContours *> contours                                              ;
  ////////////////////////////////////////////////////////////////////////////
  QSize   is = picture->Image.size()                                         ;
  QRectF  sr = canvas->mapToItem(picture,canvas->ScreenRect).boundingRect()  ;
  QRectF  si = picture->ScreenRect                                           ;
  QRect   sf (sr.left()-si.left(),sr.top()-si.top(),sr.width(),sr.height())  ;
  QRect   fr (0,0,is.width(),is.height())                                    ;
  QRect   ir = fr.intersected(sf)                                            ;
  QImage  ii = picture->Image.copy(ir)                                       ;
  ii = ii.convertToFormat(QImage::Format_ARGB32)                             ;
  ////////////////////////////////////////////////////////////////////////////
  PictureManager PM(plan)                                                    ;
  IplImage     * src     = PM . toIplImage ( ii   )                          ;
  IplImage     * grey    = PM . toGrey     ( src  )                          ;
  IplImage     * smooth  = PM . Median     ( grey )                          ;
  IplImage     * erode   = PM . Erode      ( smooth )                        ;
  IplImage     * dilate  = PM . Dilate     ( erode  )                        ;
  CvMemStorage * storage = cvCreateMemStorage(0);                            ;
  CvSeq        * contour = NULL                                              ;
  ::cvCanny (dilate , grey , 2 , 20 , 3)                                     ;
  ////////////////////////////////////////////////////////////////////////////
  ::cvFindContours(grey,storage,&contour,sizeof(CvContour)                   ,
//    CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE                                  ) ;
//    CV_RETR_LIST    ,CV_CHAIN_APPROX_NONE                                  ) ;
//    CV_RETR_CCOMP   ,CV_CHAIN_APPROX_NONE                                  ) ;
    CV_RETR_TREE    ,CV_CHAIN_APPROX_SIMPLE                                ) ;
  if (NotNull(contour))                                                      {
    QPointF RC (sf.left(),sf.top())                                          ;
    QPointF PC   = picture->ScreenRect.topLeft ( )                           ;
    PC += RC                                                                 ;
    contour = ::cvApproxPoly(contour,sizeof(CvContour),storage               ,
                CV_POLY_APPROX_DP,3,1                                      ) ;
    CvTreeNodeIterator iterator                                              ;
    ::cvInitTreeNodeIterator ( &iterator , contour , 3                     ) ;
    CvSeq * allpointsSeq = ::cvCreateSeq                                     (
                             CV_SEQ_KIND_GENERIC | CV_32SC2                  ,
                             sizeof(CvContour),sizeof(CvPoint),storage     ) ;
    while ( NULL != ( contour = (CvSeq *)cvNextTreeNode(&iterator) ) )       {
      VcfContours * vct = new VcfContours(this,canvas)                       ;
      connect(vct ,SIGNAL(Menu       (VcfContours*,QPointF))                 ,
              this,SLOT  (ContourMenu(VcfContours*,QPointF))               ) ;
      addItem ( vct , canvas )                                               ;
      scene()->addItem ( vct )                                                  ;
      vct->plan    = plan                                                    ;
      vct->Options = &Options                                                ;
      int len = contour->total                                               ;
      CvSeqReader reader                                                     ;
      CvPoint     pt                                                         ;
      ::cvStartReadSeq ( contour , &reader )                                 ;
      vct->contour . type = Graphics::Linear                                 ;
      vct->contour . begin()                                                 ;
      vct->Painter . pens [ 1 ] . setColor ( QColor(  0,  0,255) )           ;
      for (int i=0;i<len;i++)                                                {
        CV_READ_SEQ_ELEM(pt,reader)                                          ;
        QPointF P ( pt.x , pt.y )                                            ;
        P += PC                                                              ;
        P  = picture -> mapToItem ( canvas , P )                             ;
        P  = Options  . Standard  ( P          )                             ;
        ControlPoint ncp                                                     ;
        ncp.Type  = Graphics::Linear                                         ;
        ncp.Type |= Graphics::Flat                                           ;
        ncp.x     = P . x ( )                                                ;
        ncp.y     = P . y ( )                                                ;
        vct -> contour . add ( i , ncp )                                     ;
        ::cvSeqPush(allpointsSeq,&pt)                                        ;
      }                                                                      ;
      vct->contour.close(Graphics::Flat)                                     ;
      vct->Prepare()                                                         ;
      contours << vct                                                        ;
    }                                                                        ;
  }                                                                          ;
//  CvSeq * hull = ::cvConvexHull2 ( allpointsSeq , 0 , CV_CLOCKWISE , 0 )     ;
  ////////////////////////////////////////////////////////////////////////////
  PM . Release ( src    )                                                    ;
  PM . Release ( grey   )                                                    ;
  PM . Release ( smooth )                                                    ;
  PM . Release ( erode  )                                                    ;
  PM . Release ( dilate )                                                    ;
  ::cvClearMemStorage   (  storage )                                         ;
  ::cvReleaseMemStorage ( &storage )                                         ;
  ////////////////////////////////////////////////////////////////////////////
  return contours                                                            ;
}

QList<N::VcfContours *> N::VcfView::ContoursDetection (VcfPicture * picture,VcfCanvas * canvas)
{
  QList<VcfContours *> contours ;
  return contours               ;
}

void N::VcfView::KeyPoints(VcfPicture * picture)
{
  PictureManager PM ( plan )                                   ;
  FeaturePoints  FP                                            ;
  //////////////////////////////////////////////////////////////
  PM.SurfFeaturePoints ( picture -> Image , FP )               ;
  if (FP.count()<=0)                                           {
    Alert ( Error )                                            ;
    return                                                     ;
  }                                                            ;
  //////////////////////////////////////////////////////////////
  QSizeF cp ( picture->Image.size() )                          ;
  double factor = picture->Options->DPI                        ;
  double fs     = 0.015                                        ;
  cp           *= 1.27                                         ;
  cp           /= factor                                       ;
  factor       /= 2.54                                         ;
  //////////////////////////////////////////////////////////////
  Contour contour                                              ;
  QSizeF  S ( factor , factor )                                ;
  FP /= S                                                      ;
  for (int i=0;i<FP.count();i++)                               {
    FP [ i ] -> x -= cp . width  ( )                           ;
    FP [ i ] -> y -= cp . height ( )                           ;
  }                                                            ;
  contour += FP                                                ;
  contour  . thickness = ControlPoint ( fs , fs , fs )         ;
  //////////////////////////////////////////////////////////////
  VcfCanvas * vc = new VcfCanvas(this,picture,plan)            ;
  VcfPath   * vp = new VcfPath  (this,vc     ,plan)            ;
  addItem ( vc , picture )                                     ;
  addItem ( vp , vc      )                                     ;
  scene()->addItem ( vc )                                         ;
  scene()->addItem ( vp )                                         ;
  //////////////////////////////////////////////////////////////
  vc -> Options = picture->Options                             ;
  vc -> Mode    = VcfCanvas::Board                             ;
  vc -> setCanvas     ( picture->ScreenRect                  ) ;
  vc -> setZValue     ( 0.85f                                ) ;
  vc -> setOpacity    ( 0.50f                                ) ;
  //////////////////////////////////////////////////////////////
  vp -> Options = picture->Options                             ;
  vp -> setPoints     ( 2 , contour                          ) ;
  vp -> EnablePath    ( 2 , true                             ) ;
  vp -> setZValue     ( 0.90f                                ) ;
  vp -> MergePathes   ( 0                                    ) ;
  vp -> setOpacity    ( 1.00f                                ) ;
  vp -> setFlag       ( QGraphicsItem::ItemIsMovable , false ) ;
  vp -> setPenColor   ( 2 , QColor ( 240 , 240 , 128 )       ) ;
  vp -> setBrushColor ( 2 , QColor ( 255 ,  64 , 255 )       ) ;
  //////////////////////////////////////////////////////////////
  vc -> show          (                                      ) ;
  vp -> show          (                                      ) ;
  //////////////////////////////////////////////////////////////
  nConnect ( vc  ,SIGNAL(Menu          (VcfCanvas*,QPointF))   ,
             this,SLOT  (CanvasMenu    (VcfCanvas*,QPointF)) ) ;
  //////////////////////////////////////////////////////////////
  Alert               ( Done                                 ) ;
}

void N::VcfView::SaveSelection(VcfPicture * picture,VcfCanvas * canvas)
{
  QSize   is = picture->Image.size()                                        ;
  QRectF  sr = canvas->mapToItem(picture,canvas->ScreenRect).boundingRect() ;
  QRectF  si = picture->ScreenRect                                          ;
  QRect   sf (sr.left()-si.left(),sr.top()-si.top(),sr.width(),sr.height()) ;
  QRect   fr (0,0,is.width(),is.height())                                   ;
  QRectF  ir = fr.intersected(sf)                                           ;
  Sql sql = plan->sql                                                       ;
  SqlConnection SC(sql)                                                     ;
  if (SC.open("VcfGraphicsView","SaveSelection"))                           {
    QString Q                                                               ;
    if (canvas->uuid<=0)                                                    {
      canvas -> uuid = SC.Unique                                            (
                          plan->Tables[Tables::MajorUuid],"uuid") ;
      SC.assureUuid(plan->Tables[Tables::MajorUuid],canvas->uuid  ,
                    Types::Rectangle                            ) ;
      Q = sql.InsertInto                                                    (
            plan->Tables[Tables::Groups]                          ,
            5,"first","second","t1","t2","relation"                       ) ;
      SC.Prepare ( Q )                                                      ;
      SC.Bind    ( "first"    , picture ->uuid                  )           ;
      SC.Bind    ( "second"   , canvas  ->uuid                  )           ;
      SC.Bind    ( "t1"       , Types::Picture        )           ;
      SC.Bind    ( "t2"       , Types::Rectangle      )           ;
      SC.Bind    ( "relation" , Groups::Subordination )           ;
      SC.Exec    (   )                                                      ;
      Q = sql.InsertInto                                                    (
            plan->Tables[Tables::Rectangles]                      ,
            5,"uuid","x","y","w","h"                                      ) ;
    } else                                                                  {
      Q = sql.Update                                                        (
            plan->Tables[Tables::Rectangles]                      ,
              QString("where uuid = :UUID")                                 ,
              4,"x","y","w","h"                                           ) ;
    }                                                                       ;
    SC.Prepare ( Q )                                                        ;
    SC.Bind    ( "uuid" , canvas->uuid  )                                   ;
    SC.Bind    ( "x"    , ir.left   ( ) )                                   ;
    SC.Bind    ( "y"    , ir.top    ( ) )                                   ;
    SC.Bind    ( "w"    , ir.width  ( ) )                                   ;
    SC.Bind    ( "h"    , ir.height ( ) )                                   ;
    SC.Exec    (   )                                                        ;
    SC.close()                                                              ;
  }                                                                         ;
  SC.remove()                                                               ;
  Alert ( Done )                                                            ;
}

void N::VcfView::RemoveSelection(VcfPicture * picture,VcfCanvas * canvas)
{
  if (canvas->uuid<=0) return                                               ;
  Sql sql = plan->sql                                                       ;
  SqlConnection SC(sql)                                                     ;
  if (SC.open("VcfGraphicsView","RemoveSelection"))                         {
    QString Q                                                               ;
    Q = sql.DeleteFrom(plan->Tables[Tables::Rectangles]           ,
            QString("where uuid = %1").arg(canvas->uuid)                  ) ;
    SC.Query(Q)                                                             ;
    Q = sql.SelectFrom("id",plan->Tables[Tables::Rectangles]      ,
            QString("where uuid = %1").arg(canvas->uuid)                  ) ;
    if (SC.Fetch(Q)) { ; } else                                             {
      Q = sql.DeleteFrom(plan->Tables[Tables::Groups]             ,
              QString("where second = %1").arg(canvas->uuid)              ) ;
      SC.Query(Q)                                                           ;
      Q = sql.DeleteFrom(plan->Tables[Tables::MajorUuid]          ,
              QString("where uuid = %1").arg(canvas->uuid)                ) ;
      SC.Query(Q)                                                           ;
    }                                                                       ;
    SC.close()                                                              ;
  }                                                                         ;
  SC.remove()                                                               ;
  Alert ( Done )                                                            ;
}

void N::VcfView::Selection(VcfItem * item,QRectF Screen)
{
  VcfCanvas * canvas = NewCanvas ( item )      ;
  canvas -> Mode = VcfCanvas::Border           ;
  canvas -> setCanvas  ( Screen              ) ;
  canvas -> setZValue  ( item->zValue()+0.01 ) ;
  canvas -> setOpacity ( 0.50f               ) ;
}

void N::VcfView::Canvas(VcfItem * item,QRectF Screen)
{
  VcfCanvas * canvas = NewCanvas ( item )      ;
  canvas -> Mode = VcfCanvas::Board            ;
  canvas -> setCanvas  ( Screen              ) ;
  canvas -> setZValue  ( item->zValue()+0.01 ) ;
  canvas -> setOpacity ( 0.25f               ) ;
}

void N::VcfView::CanvasMenu(VcfCanvas * canvas,QPointF pos)
{
  nScopedMenu ( mm , this )                                             ;
  QMenu      * me                                                       ;
  QMenu      * mp                                                       ;
  QMenu      * mv                                                       ;
  QMenu      * mc                                                       ;
  QAction    * aa                                                       ;
  VcfPicture * picture = VcfCasting(VcfPicture,Maps[canvas])            ;
  QList<VcfContours *> contours = ItemContours(canvas)                  ;
  canvas -> addAdjustMenu   ( mm )                                      ;
  me = mm . addMenu(tr("Edit"                           ))              ;
  mm . add(me,20001,tr("Delete"                         ))              ;
  mm . add(me,20002,tr("Remove from database"           ))              ;
  if (NotNull(picture))                                                 {
    QMenu * msobel                                                      ;
    mp = mm.addMenu(tr("Pictures"                       ))              ;
    mm.add(mp,30001,tr("Crop image within selected area"))              ;
    mm.add(mp,30002,tr("Save selection"                 ))              ;
    mm.addSeparator   (mp                                )              ;
    msobel = mm.addMenu(mp,tr("Sobel"))                                 ;
    mm.add(msobel,30101,tr("Sobel operator"             ))              ;
    mm.add(msobel,30102,tr("Vertical Sobel operator"    ))              ;
    mm.add(msobel,30103,tr("Horizontal Sobel operator"  ))              ;
    mm.add(mp,30003,tr("Canny operator"                 ))              ;
    mm.add(mp,30004,tr("Laplace operator"               ))              ;
    mv = mm.addMenu(tr("Vectorizations"                 ))              ;
    mm.add(mv,40001,tr("Canny edge detection"           ))              ;
    mm.add(mv,40002,tr("Sobel edge detection"           ))              ;
    mm.add(mv,40003,tr("Laplace edge detection"         ))              ;
    mm.addSeparator   (mv                                )              ;
    mm.add(mv,40004,tr("Contours detection"             ))              ;
  }                                                                     ;
  if (contours.count()>0)                                               {
    mc = mm.addMenu(tr("Shapes"                         ))              ;
    mm.add(mc,50001,tr("Save"                           ))              ;
    mm.add(mc,50002,tr("Delete all"                     ))              ;
  }                                                                     ;
  canvas -> addFontMenu     ( mm )                                      ;
  canvas -> addPenMenu      ( mm )                                      ;
  canvas -> addBrushMenu    ( mm )                                      ;
  canvas -> addGradientMenu ( mm )                                      ;
  mm . setFont ( plan )                                                 ;
  aa = mm.exec(QCursor::pos()   )                                       ;
  switch (mm[aa])                                                       {
    case 10001                                                          :
      canvas->setFlag( QGraphicsItem::ItemIsMovable , aa->isChecked() ) ;
    break                                                               ;
    case 10002                                                          :
      canvas -> Scaling = aa->isChecked()                               ;
    break                                                               ;
    case 11001                                                          :
      canvas -> SavePen ( )                                             ;
    break                                                               ;
    case 11002                                                          :
      EditPenColor   ( canvas )                                         ;
    break                                                               ;
    case 12001                                                          :
      canvas -> SaveBrush ( )                                           ;
    break                                                               ;
    case 12002                                                          :
      EditBrushColor ( canvas )                                         ;
    break                                                               ;
    case 13001                                                          :
      canvas -> SaveGradient ( )                                        ;
    break                                                               ;
    case 13002                                                          :
      EditGradient   ( canvas )                                         ;
    break                                                               ;
    case 20001                                                          :
      takeItem (canvas)                                                 ;
    break                                                               ;
    case 20002                                                          :
      if (NotNull(picture)) RemoveSelection ( picture , canvas )        ;
      takeItem (canvas)                                                 ;
    break                                                               ;
    case 30001                                                          :
      if (NotNull(picture)) CropPicture     ( picture , canvas )        ;
    break                                                               ;
    case 30002                                                          :
      SaveSelection     ( picture , canvas )                            ;
    break                                                               ;
    case 30003                                                          :
      CannyPicture      ( picture , canvas )                            ;
    break                                                               ;
    case 30004                                                          :
      LaplacePicture    ( picture , canvas )                            ;
    break                                                               ;
    case 30101                                                          :
      SobelPicture      ( picture , canvas )                            ;
    break                                                               ;
    case 30102                                                          :
      VerticalSobel     ( picture , canvas )                            ;
    break                                                               ;
    case 30103                                                          :
      HorizontalSobel   ( picture , canvas )                            ;
    break                                                               ;
    case 40001                                                          :
      CannyDetection    ( picture , canvas )                            ;
    break                                                               ;
    case 40002                                                          :
      SobelDetection    ( picture , canvas )                            ;
    break                                                               ;
    case 40003                                                          :
      LaplaceDetection  ( picture , canvas )                            ;
    break                                                               ;
    case 40004                                                          :
      ContoursDetection ( picture , canvas )                            ;
    break                                                               ;
    case 50001                                                          :
      canvas -> SaveContours ( )                                        ;
    break                                                               ;
    case 50002                                                          :
      for (int i=0;i<contours.count();i++)                              {
        takeItem ( contours [ i ] )                                     ;
      }                                                                 ;
    break                                                               ;
    default                                                             :
      canvas->MenuCommands(mm[aa],aa)                                   ;
    break                                                               ;
  }                                                                     ;
  repaint ( )                                                           ;
}

void N::VcfView::ContourMenu(VcfContours * contour,QPointF pos)
{
  nScopedMenu ( mm , this )                             ;
  QAction    * aa     = NULL                            ;
  VcfCanvas  * canvas = NULL                            ;
  QList<VcfContours *> contours                         ;
  canvas = VcfCasting(VcfCanvas,Maps[contour])          ;
  mm.add(101,QIcon(":/images/save.png"  ),tr("Save"  )) ;
  mm.add(108,QIcon(":/images/delete.png"),tr("Delete")) ;
  if (NotNull(canvas))                                  {
    contours = ItemContours(canvas)                     ;
    if (contours.count()>1)                             {
      mm.addSeparator()                                 ;
      mm.add(109,tr("Delete all others"))               ;
    }                                                   ;
  }                                                     ;
  mm . setFont ( plan )                                 ;
  aa = mm.exec(QCursor::pos())                          ;
  if (IsNull(aa)) return                                ;
  switch (mm[aa])                                       {
    case 101                                            :
      StoreContours ( canvas , contour )                ;
    break                                               ;
    case 108                                            :
      takeItem(contour)                                 ;
    break                                               ;
    case 109                                            :
      for (int i=0;i<contours.count();i++)              {
        VcfContours * vc = contours[i]                  ;
        if (vc!=contour) takeItem(vc)                   ;
      }                                                 ;
    break                                               ;
  }                                                     ;
}

void N::VcfView::ShapeName(VcfCanvas * canvas,QString name)
{
  if (name.length()<=0) return              ;
  GraphicsManager GM ( plan )               ;
  EnterSQL ( SC , plan->sql )               ;
    QList<VcfContours *> contours           ;
    contours = ItemContours(canvas)         ;
    if (contours.count()>0)                 {
      GM.NewShape(SC,name,plan->LanguageId) ;
      for (int i=0;i<contours.count();i++)  {
        VcfContours * vc = contours[i]      ;
      }                                     ;
      Notify("Contours",QString::number(contours.count())) ;
    }                                       ;
  LeaveSQL ( SC , plan->Sql )               ;
}

void N::VcfView::EditPenColor(VcfCanvas * canvas)
{
  QPointF          cp  = canvas->ScreenRect.bottomLeft() ;
  VcfColorPicker * vcp = NewColorPicker ( NULL )         ;
  vcp -> bundle =  canvas                                ;
  vcp -> pen    = &canvas -> Painter . pens [ 0 ]        ;
  vcp -> color  =  vcp    -> pen   -> color (   )        ;
  vcp -> setZValue  ( canvas->zValue() + 0.01 )          ;
  vcp -> setOpacity ( 1.0                     )          ;
  vcp -> Prepare    (                         )          ;
  cp   = canvas  -> mapToScene ( cp )                    ;
  cp   = Options  . Standard   ( cp )                    ;
  vcp -> setCorner             ( cp )                    ;
}

void N::VcfView::EditBrushColor(VcfCanvas * canvas)
{
  QPointF          cp  = canvas->ScreenRect.bottomLeft() ;
  VcfColorPicker * vcp = NewColorPicker ( NULL )         ;
  vcp -> bundle =  canvas                                ;
  vcp -> brush  = &canvas -> Painter . brushes [ 0 ]     ;
  vcp -> color  =  vcp    -> brush  -> color   (   )     ;
  vcp -> setZValue  ( canvas->zValue() + 0.01 )          ;
  vcp -> setOpacity ( 1.0                     )          ;
  vcp -> Prepare    (                         )          ;
  cp   = canvas  -> mapToScene ( cp )                    ;
  cp   = Options  . Standard   ( cp )                    ;
  vcp -> setCorner             ( cp )                    ;
}

void N::VcfView::EditGradient(VcfCanvas * canvas)
{
  QPointF          cp  = canvas->ScreenRect.bottomLeft() ;
  VcfColorPicker * vcp = NewColorPicker ( NULL )         ;
  vcp -> bundle   =  canvas                              ;
  vcp -> gradient = &canvas -> Painter . gradients [ 0 ] ;
  vcp -> setZValue  ( canvas->zValue() + 0.01 )          ;
  vcp -> setOpacity ( 1.0                     )          ;
  vcp -> Prepare    (                         )          ;
  cp   = canvas  -> mapToScene ( cp )                    ;
  cp   = Options  . Standard   ( cp )                    ;
  vcp -> setCorner             ( cp )                    ;
}

void N::VcfView::StoreContours(VcfCanvas * canvas,VcfContours * contour)
{
  VcfInterface      * vine = NULL                 ;
  ContourController * cc   = NULL                 ;
  vine = new VcfInterface      ( this , canvas )  ;
  cc   = new ContourController ( NULL , plan   )  ;
  QRect RN = cc->geometry()                       ;
  QRect RF( 0, 0,RN.width()+8,RN.height()+32)     ;
  QRect RC( 4,28,RN.width()  ,RN.height()   )     ;
  QRect HR( 4, 2,RN.width()  ,            24)     ;
  QPointF GP (0.0,0.0)                            ;
  vine -> showHeader = true                       ;
  vine -> HeaderRect = HR                         ;
  vine -> Title      = cc->windowTitle()          ;
  vine -> setOptions      ( Options,true       )  ;
  vine -> setZValue       ( 1.00f              )  ;
  vine -> setWidget       ( cc  , GP , RF , RC )  ;
  vine -> adjustTransform (                    )  ;
  cc   -> canvas  = canvas                        ;
  cc   -> contour = contour                       ;
  Append ( vine , canvas )                        ;
  connect(vine,SIGNAL(Finish(VcfItem*))           ,
          this,SLOT  (Remove(VcfItem*))         ) ;
  connect(cc  ,SIGNAL(Finish())                   ,
          vine,SLOT  (Finish())                 ) ;
  cc   -> Display ( )                             ;
}

void N::VcfView::EditFlags(QGraphicsItem * item)
{
  QRectF R (12,12,320,height()-32)               ;
  VcfFlags * Flags = NULL                        ;
  Flags = new VcfFlags ( this  , NULL )          ;
  scene ( ) -> addItem ( Flags        )          ;
  addItem              ( Flags , NULL )          ;
  Flags -> plan    = plan                        ;
  Flags -> Scaling = true                        ;
  Flags -> setOptions ( Options , true )         ;
  Flags -> Initialize ( item    , R    )         ;
  connect(Flags , SIGNAL (Finish(VcfItem*))      ,
          this  , SLOT   (Remove(VcfItem*))    ) ;
}

#ifdef QT_OPENGL_LIB

void N::VcfView::setupOpenGL(void)
{
#ifdef USE_QOpenGL
  QOpenGLWidget * gl                                           ;
  gl = new QOpenGLWidget (                                   ) ;
#else
  QGLWidget     * gl                                           ;
  gl = new QGLWidget     ( QGLFormat ( QGL::DoubleBuffer       |
                                       QGL::DepthBuffer        |
                                       QGL::Rgba               |
                                       QGL::AlphaChannel       |
                                       QGL::AccumBuffer        |
                                       QGL::StencilBuffer      |
                                       QGL::SampleBuffers  ) ) ;
  gl -> setAttribute     ( Qt::WA_InputMethodEnabled , true  ) ;
#endif
  setFunction            ( 100001 , true                     ) ;
  setViewport            ( gl                                ) ;
  setViewportUpdateMode  ( QGraphicsView::FullViewportUpdate ) ;
  setScene               ( Scene                             ) ;
  setTransform           ( Transform                         ) ;
  setScrollBarPolicy     ( Qt::ScrollBarAlwaysOff            ) ;
}

void N::VcfView::Rendering(void)
{
}

#endif

void N::VcfView::drawBackground (QPainter * p,const QRectF & r)
{
  #ifdef QT_OPENGL_LIB
  if ( isFunction ( 100001 ) )                           {
    p -> save                     (                    ) ;
    p -> beginNativePainting      (                    ) ;
    ::glPushAttrib                ( GL_ALL_ATTRIB_BITS ) ;
    ::glMatrixMode                ( GL_PROJECTION      ) ;
    ::glPushMatrix                (                    ) ;
    Rendering                     (                    ) ;
    ::glMatrixMode                ( GL_PROJECTION      ) ;
    ::glPopMatrix                 (                    ) ;
    ::glPopAttrib                 (                    ) ;
    p -> endNativePainting        (                    ) ;
    p -> restore                  (                    ) ;
  } else                                                 {
    QGraphicsView::drawBackground ( p , r              ) ;
  }                                                      ;
  #else
  QGraphicsView::drawBackground   ( p , r              ) ;
  #endif
}
