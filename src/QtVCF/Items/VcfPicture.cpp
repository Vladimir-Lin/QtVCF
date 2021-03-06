#include <qtvcf.h>

N::VcfPicture:: VcfPicture   ( VcfHeadParaments  )
              : VcfRectangle ( parent , item , p )
              , Object       ( 0 , Types::None   )
              , PictureDPI   ( 96.0              )
{
  Transform . reset ()                                ;
  Printable = true                                    ;
  Scaling   = false                                   ;
  setFlag(ItemIsSelectable        ,true )             ;
  setFlag(ItemIsFocusable         ,true )             ;
  setFlag(ItemIsMovable           ,true )             ;
  setFlag(ItemSendsGeometryChanges,true )             ;
  setFlag(ItemClipsToShape        ,false)             ;
  setFlag(ItemClipsChildrenToShape,false)             ;
  Painter . addMap ( "Border" , 0                   ) ;
  Painter . addPen ( 0 , QColor ( 224 , 224 , 224 ) ) ;
  Painter . pens [0] . setStyle ( Qt::DotLine       ) ;
}

N::VcfPicture::~VcfPicture(void)
{
  if (IsNull(Options)  ) return ;
  if (!Options->Private) return ;
  delete Options                ;
  Options = NULL                ;
}

void N::VcfPicture::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
  emit Menu(this,event->pos());
  event->accept();
}

void N::VcfPicture::paint(QPainter * painter,const QStyleOptionGraphicsItem * option,QWidget * widget)
{
  Paint(painter,ScreenRect,false,true);
  if (isSelected()) PaintBorder(painter,ScreenRect,false,true);
}

void N::VcfPicture::Paint(QPainter * painter,QRectF Region,bool clip,bool color)
{
  if (clip) PaintImageClip (painter,Region,clip,color) ;
       else PaintImage     (painter,Region,clip,color) ;
}

void N::VcfPicture::PaintImage(QPainter * p,QRectF Region,bool clip,bool color)
{
  p->drawImage(ScreenRect,Image);
}

void N::VcfPicture::PaintImageClip(QPainter * p,QRectF Region,bool clip,bool color)
{
  QPointF ps(0,0);
  ps = mapToScene(ps);
  QPointF px(ps.x()-Region.left(),ps.y()-Region.top());
  QTransform TX = transform().inverted();
  px = TX.map(px);
  p->setTransform(transform());
  p->translate(px);
  p->drawImage(ScreenRect,Image);
}

void N::VcfPicture::PaintBorder(QPainter * p,QRectF Region,bool clip,bool color)
{
  pushPainters         ( p                         ) ;
  Painter . drawBorder ( p , "Border" , ScreenRect ) ;
  popPainters          ( p                         ) ;
}

void N::VcfPicture::LoadImage(SUID puid)
{
  Mutex . lock   ()                      ;
  SqlConnection  SC (plan->sql)          ;
  PictureManager PM (plan     )          ;
  if (SC.open("VcfPicture","LoadImage")) {
    QImage *  I = NULL                   ;
    I = PM.Picture(SC,puid)              ;
    if (I!=NULL)                         {
      uuid  = puid                       ;
      Image = *I                         ;
      delete I                           ;
    }                                    ;
    SC.close()                           ;
  }                                      ;
  SC.remove()                            ;
  Mutex . unlock ()                      ;
}

void N::VcfPicture::setCenter(QPointF center)
{
  if (IsNull(Options)) return                              ;
  PaperPos = center                                        ;
  QGraphicsItem::setPos(Options->position(center))         ;
  QTransform T                                             ;
  T.reset()                                                ;
  qreal sx = Options->DPI                                  ;
  qreal sy = Options->DPI                                  ;
  sx /= PictureDPI                                         ;
  sy /= PictureDPI                                         ;
  T = T.scale(sx,sy)                                       ;
  Transform = T                                            ;
  setTransform(T)                                          ;
  QSize S(Image.width(),Image.height())                    ;
  QPointF C(S.width()/2,S.height()/2)                      ;
  ScreenRect.setLeft     (-C.x     ())                     ;
  ScreenRect.setTop      (-C.y     ())                     ;
  ScreenRect.setWidth    (S.width  ())                     ;
  ScreenRect.setHeight   (S.height ())                     ;
  QRectF SR = mapToScene (ScreenRect ).boundingRect()      ;
  PaperRect = Options -> Standard (SR)                     ;
  setToolTip                                               (
    tr("Picture UUID : %1\n"
       "%2 x %3 pixels\n"
       "%4 DPI\n"
       "%5 x %6 cm\n"
       "Center : %7 x %8 cm"                      )
    .arg(uuid                                     )
    .arg(S        .width()).arg(S        .height())
    .arg(PictureDPI                               )
    .arg(PaperRect.width()).arg(PaperRect.height())
    .arg(PaperPos .x    ()).arg(PaperPos .y     ())
  )                                                        ;
  prepareGeometryChange  (           )                     ;
}

QVariant N::VcfPicture::itemChange(GraphicsItemChange change,const QVariant & value)
{
  switch (change)                                       {
    case ItemPositionChange                             :
    case ItemPositionHasChanged                         :
      if (NotNull(Options))                             {
        QPointF scenePos = value.toPointF(  )           ;
        PaperPos = Options->Standard(scenePos)          ;
        setToolTip                                      (
          tr("Picture UUID : %1\n"
             "%2 x %3 pixels\n"
             "%4 DPI\n"
             "%5 x %6 cm\n"
             "Center : %7 x %8 cm"                      )
          .arg(uuid                                     )
          .arg(Image    .width()).arg(Image    .height())
          .arg(PictureDPI                               )
          .arg(PaperRect.width()).arg(PaperRect.height())
          .arg(PaperPos .x    ()).arg(PaperPos .y     ())
        )                                               ;
        QToolTip::showText(QCursor::pos(),toolTip())    ;
      }                                                 ;
    break                                               ;
    case ItemSelectedHasChanged                         :
      DeleteGadgets()                                   ;
    break                                               ;
  }                                                     ;
  return QGraphicsItem::itemChange(change,value)        ;
}

void N::VcfPicture::setCornerCursor(int Corner)
{
  switch (Corner)                      {
    case NoSide                        :
      setCursor(Qt::ArrowCursor     )  ;
    break                              ;
    case TopLeft                       :
      setCursor(Qt::SizeFDiagCursor )  ;
    break                              ;
    case TopRight                      :
      setCursor(Qt::SizeBDiagCursor )  ;
    break                              ;
    case BottomLeft                    :
      setCursor(Qt::SizeBDiagCursor )  ;
    break                              ;
    case BottomRight                   :
      setCursor(Qt::SizeFDiagCursor )  ;
    break                              ;
    case LeftSide                      :
      setCursor(Qt::SizeHorCursor   )  ;
    break                              ;
    case RightSide                     :
      setCursor(Qt::SizeHorCursor   )  ;
    break                              ;
    case TopSide                       :
      setCursor(Qt::SizeVerCursor   )  ;
    break                              ;
    case BottomSide                    :
      setCursor(Qt::SizeVerCursor   )  ;
    break                              ;
    case Inside                        :
      setCursor(Qt::ClosedHandCursor)  ;
    break                              ;
  }                                    ;
}

void N::VcfPicture::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
  QGraphicsItem::mouseDoubleClickEvent(event);
}

void N::VcfPicture::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  scalePressEvent   ( event ) ;
}

void N::VcfPicture::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
  scaleMoveEvent    ( event ) ;
}

void N::VcfPicture::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  scaleReleaseEvent ( event ) ;
}

bool N::VcfPicture::showMenu(QGraphicsView * view,QPoint global)
{
  nScopedMenu ( mm , view )                          ;
  bool movable = IsMask (flags(),ItemIsMovable)      ;
  QMenu      * ma = NULL                             ;
  QMenu      * me = NULL                             ;
  QMenu      * mp = NULL                             ;
  QMenu      * mc = NULL                             ;
  QAction    * a  = NULL                             ;
  me = mm.addMenu(tr("Edit"))                        ;
  if (uuid> 0) mm.add(me,23,tr("Drag" ))             ;
  if (uuid<=0) mm.add(me,24,tr("Store" ))            ;
      mm.addSeparator(me)                            ;
  if (uuid> 0) mm.add(me,25,tr("Load selections"))   ;
      mm.add(me,21,tr("Create selection area"))      ;
      mm.add(me,22,tr("Create canvas"))              ;
      mm.addSeparator(me)                            ;
      mm.add(me,26,tr("Save as" ))                   ;
      mm.add(me, 1,tr("Delete" ))                    ;
  ma = mm.addMenu(tr("Adjustments"))                 ;
  a  = mm.add(ma, 2,tr("Overlay"))                   ;
  a->setCheckable(true); a->setChecked(Overlay)      ;
  a  = mm.add(ma, 3,tr("Movable"))                   ;
  a->setCheckable(true); a->setChecked(movable)      ;
  a  = mm.add(ma, 4,tr("Resizable"))                 ;
  a->setCheckable(true); a->setChecked(Scaling)      ;
      mm.addSeparator(ma)                            ;
      mm.add(ma,11,tr("DPI"    ))                    ;
      mm.add(ma,12,tr("Z Level"))                    ;
      mm.add(ma,13,tr("Opacity"))                    ;
      mm.add(ma,14,tr("Rotate" ))                    ;
      mm.add(ma,15,tr("Normal" ))                    ;
  mc = mm.addMenu(tr("Channels"))                    ;
  mm.add(mc,75,tr("Extract red channel"  ))          ;
  mm.add(mc,76,tr("Extract green channel"))          ;
  mm.add(mc,77,tr("Extract blue channel" ))          ;
  mm.add(mc,78,tr("Extract alpha channel"))          ;
  mm.addSeparator(mc)                                ;
  mm.add(mc,79,tr("Picture colors join color group"));
  mp = mm.addMenu(tr("Process"))                     ;
      mm.add(mp,81,tr("Filters"))                    ;
  if (plan->classifiers.count()>0)                   {
      mm.add(mp,52,tr("Human faces detection"))      ;
      mm.add(mp,53,tr("Human faces analysis" ))      ;
  }                                                  ;
      mm.addSeparator(mp)                            ;
      mm.add(mp,82,tr("Grey image"     ))            ;
      mm.add(mp,83,tr("Invert RGB"     ))            ;
      mm.add(mp,84,tr("Median smooth"  ))            ;
      mm.add(mp,85,tr("Gaussian smooth"))            ;
      mm.add(mp,86,tr("Blur"           ))            ;
      mm.addSeparator(mp                )            ;
      mm.add(mp,87,tr("Erode"          ))            ;
      mm.add(mp,88,tr("Dilate"         ))            ;
      mm.addSeparator(mp)                            ;
      mm.add(mp,89,tr("Contrast"))                   ;
      mm.add(mp,90,tr("Unsharp mask"))               ;
      mm.addSeparator(mp)                            ;
      mm.add(mp,91,tr("Color distribution"))         ;
      mm.add(mp,92,tr("Transform"))                  ;
      mm.addSeparator(mp)                            ;
      mm.add(mp,93,tr("Feature points"))             ;
      mm.setFont(plan)                               ;
  a = mm.exec(global)                                ;
  if (IsNull(a)) return false                        ;
  PictureManager PM ( plan )                         ;
  QImage   * II                                      ;
  IplImage * image                                   ;
  IplImage * result                                  ;
  switch (mm[a])                                     {
    case  1                                          :
      emit Delete (this)                             ;
    break                                            ;
    case  2                                          :
      Overlay   = a->isChecked()                     ;
    break                                            ;
    case  3                                          :
      movable   = a->isChecked()                     ;
      setFlag ( ItemIsMovable , movable )            ;
    break                                            ;
    case  4                                          :
      Scaling = a->isChecked()                       ;
    break                                            ;
    case 11                                          :
      DeleteGadgets  ()                              ;
      AttachDPI      ()                              ;
    break                                            ;
    case 12                                          :
      DeleteGadgets  ()                              ;
      AttachZLevel   ()                              ;
    break                                            ;
    case 13                                          :
      DeleteGadgets  ()                              ;
      AttachOpacity  ()                              ;
    break                                            ;
    case 14                                          :
      DeleteGadgets  ()                              ;
      AttachRotation ()                              ;
    break                                            ;
    case 15                                          :
      DeleteGadgets  ()                              ;
      NormalTransform()                              ;
    break                                            ;
    case 21                                          :
      emit Selection ( this , ScreenRect )           ;
    break                                            ;
    case 22                                          :
      emit Canvas    ( this , ScreenRect )           ;
    break                                            ;
    case 23                                          :
      Drag  ( )                                      ;
    break                                            ;
    case 24                                          :
      Store ( )                                      ;
    break                                            ;
    case 25                                          :
      emit LoadSelections ( this )                   ;
    break                                            ;
    case 26                                          :
      SaveAs              (      )                   ;
    break                                            ;
    case 52                                          :
      emit Faces          ( this )                   ;
    break                                            ;
    case 53                                          :
      emit FacesAnalysis  ( this )                   ;
    break                                            ;
    case 75                                          :
      emit Channel        ( this , 0 )               ;
    break                                            ;
    case 76                                          :
      emit Channel        ( this , 1 )               ;
    break                                            ;
    case 77                                          :
      emit Channel        ( this , 2 )               ;
    break                                            ;
    case 78                                          :
      emit Channel        ( this , 3 )               ;
    break                                            ;
    case 79                                          :
      JoinColorGroup      (          )               ;
    break                                            ;
    case 81                                          :
      emit Process        ( this )                   ;
    break                                            ;
    case 82                                          :
      image  = PM.toIplImage( Image  )               ;
      result = PM.toGrey    ( image  )               ;
      II     = PM.toImage   ( result )               ;
      Image  = *II                                   ;
      delete II                                      ;
      PM . Release ( image  )                        ;
      PM . Release ( result )                        ;
      update ( )                                     ;
    break                                            ;
    case 83                                          :
      Image.invertPixels ()                          ;
      update ( )                                     ;
    break                                            ;
    case 84                                          :
      image  = PM.toIplImage ( Image  )              ;
      result = PM.Median     ( image  )              ;
      II     = PM.toImage    ( result )              ;
      if (NotNull(II)) Image = *II                   ;
      PM     . Release       ( image  )              ;
      PM     . Release       ( result )              ;
      if (NotNull(II)) delete II                     ;
      update ( )                                     ;
    break                                            ;
    case 85                                          :
      image  = PM.toIplImage ( Image  )              ;
      result = PM.Gaussian   ( image  )              ;
      II     = PM.toImage    ( result )              ;
      if (NotNull(II)) Image = *II                   ;
      PM     . Release       ( image  )              ;
      PM     . Release       ( result )              ;
      if (NotNull(II)) delete II                     ;
      update ( )                                     ;
    break                                            ;
    case 86                                          :
      image  = PM.toIplImage ( Image  )              ;
      result = PM.Blur       ( image  )              ;
      II     = PM.toImage    ( result )              ;
      if (NotNull(II)) Image = *II                   ;
      PM     . Release       ( image  )              ;
      PM     . Release       ( result )              ;
      if (NotNull(II)) delete II                     ;
      update ( )                                     ;
    break                                            ;
    case 87                                          :
      image  = PM.toIplImage ( Image  )              ;
      result = PM.Erode      ( image  )              ;
      II     = PM.toImage    ( result )              ;
      if (NotNull(II)) Image = *II                   ;
      PM     . Release       ( image  )              ;
      PM     . Release       ( result )              ;
      if (NotNull(II)) delete II                     ;
      update ( )                                     ;
    break                                            ;
    case 88                                          :
      image  = PM.toIplImage ( Image  )              ;
      result = PM.Dilate     ( image  )              ;
      II     = PM.toImage    ( result )              ;
      if (NotNull(II)) Image = *II                   ;
      PM     . Release       ( image  )              ;
      PM     . Release       ( result )              ;
      if (NotNull(II)) delete II                     ;
      update ( )                                     ;
    break                                            ;
    case 89                                          :
      DeleteGadgets  ()                              ;
      AdjustContrast ()                              ;
    break                                            ;
    case 90                                          :
      DeleteGadgets  ()                              ;
      UnsharpMask    ()                              ;
    break                                            ;
    case 91                                          :
      emit ColorDistribution ( this )                ;
    break                                            ;
    case 92                                          :
      emit TransformPicture  ( this )                ;
    break                                            ;
    case 93                                          :
      emit KeyPoints         ( this )                ;
    break                                            ;
  }                                                  ;
  return true                                        ;
}

QRectF N::VcfPicture::PanelRect(void)
{
  QPointF X(3.0,0.40);
  QPointF TL = ScreenRect . topLeft () ;
  TL = mapToScene(TL)        ;
  TL = Options->Standard(TL) ;
  QRectF Z (TL.x(),TL.y()-X.y(),X.x(),X.y()) ;
  Z  = Options->Region(Z) ;
  Z  = mapFromScene(Z).boundingRect ( ) ;
  return QRectF(ScreenRect.left(),Z.top(),ScreenRect.width(),Z.height()) ;
}

QRectF N::VcfPicture::CenterRect(void)
{
  QPointF X(3.0,3.0)                             ;
  QPointF H = X / 2                              ;
  QPointF C = ScreenRect.center()                ;
  C = mapToScene(C)                              ;
  C = Options->Standard(C)                       ;
  QRectF Z (C.x()-H.x(),C.y()-H.y(),X.x(),X.y()) ;
  Z  = Options->Region(Z)                        ;
  return Z                                       ;
}

void N::VcfPicture::MountZLevel(QGraphicsProxyWidget * proxy,QSlider * slider)
{
  Proxys  [1] = proxy       ;
  Widgets [1] = slider      ;
  QRectF R = PanelRect()    ;
  proxy->setGeometry(R    ) ;
  proxy->setZValue  (0.90f) ;
  proxy->setOpacity (0.50f) ;
}

void N::VcfPicture::MountOpacity(QGraphicsProxyWidget * proxy,QSlider * slider)
{
  Proxys  [2] = proxy       ;
  Widgets [2] = slider      ;
  QRectF R = PanelRect()    ;
  proxy->setGeometry(R)     ;
  proxy->setZValue  (0.90f) ;
  proxy->setOpacity (0.50f) ;
}

void N::VcfPicture::MountRotation(QGraphicsProxyWidget * proxy,QDial * dial)
{
  Proxys  [3] = proxy       ;
  Widgets [3] = dial        ;
  QRectF R = CenterRect()   ;
  qreal  z = zValue()       ;
  z += 0.90f                ;
  if (z>1.0) z = 1.0        ;
  proxy->setGeometry(R)     ;
  proxy->setZValue  (z    ) ;
  proxy->setOpacity (0.75f) ;
}

void N::VcfPicture::RotationUpdated(void)
{
  QTransform T = Transform ;
  T = T.rotate(Angle)      ;
  setTransform(T)          ;
}

void N::VcfPicture::NormalTransform(void)
{
  QTransform T            ;
  T.reset()               ;
  Angle = 0.0             ;
  qreal sx = Options->DPI ;
  qreal sy = Options->DPI ;
  sx /= PictureDPI        ;
  sy /= PictureDPI        ;
  T = T.scale(sx,sy)      ;
  Transform = T           ;
  setTransform(T)         ;
}

void N::VcfPicture::AttachDPI(void)
{
  QDoubleSpinBox * dpi = new QDoubleSpinBox();
  QGraphicsProxyWidget * proxy  = new QGraphicsProxyWidget(this);
  proxy->setWidget(dpi);
  dpi->setRange(1,9600);
  dpi->setValue(PictureDPI);
  dpi->setSingleStep(1);
  connect(dpi,SIGNAL(valueChanged(double)),this,SLOT(modifyDPI(double)));
  Proxys  [4] = proxy       ;
  Widgets [4] = dpi         ;
  QRectF R = PanelRect()    ;
  proxy->setGeometry(R    ) ;
  proxy->setZValue  (0.90f) ;
  proxy->setOpacity (0.50f) ;
  QFont Font = plan->fonts[Fonts::ComboBox] ;
  Font.setPixelSize(R.height());
  dpi->setFont(Font);
}

void N::VcfPicture::modifyDPI(double dpi)
{
  QTransform T                                        ;
  T.reset()                                           ;
  qreal sx = Options->DPI                             ;
  qreal sy = Options->DPI                             ;
  PictureDPI = dpi                                    ;
  sx /= PictureDPI                                    ;
  sy /= PictureDPI                                    ;
  T = T.scale(sx,sy)                                  ;
  Transform = T                                       ;
  T = T.rotate(Angle)                                 ;
  setTransform(T)                                     ;
  QRectF SR = mapToScene (ScreenRect ).boundingRect() ;
  PaperRect = Options -> Standard (SR)                ;
  setToolTip                                          (
    tr("Picture UUID : %1\n"
       "%2 x %3 pixels\n"
       "%4 DPI\n"
       "%5 x %6 cm\n"
       "Center : %7 x %8 cm"                      )
    .arg(uuid                                     )
    .arg(Image    .width()).arg(Image    .height())
    .arg(PictureDPI                               )
    .arg(PaperRect.width()).arg(PaperRect.height())
    .arg(PaperPos .x    ()).arg(PaperPos .y     ())
  )                                                   ;
  prepareGeometryChange  (           )                ;
}

void N::VcfPicture::Drag(void)
{
  if (uuid<=0) return                                      ;
  QMimeData * mime = new QMimeData()                       ;
  QByteArray  data((const char *)&uuid,sizeof(SUID))       ;
  mime->setData("picture/uuid",data)                       ;
  QDrag * drag = new QDrag (GraphicsView())                ;
  Qt::DropAction dropAction                                ;
  drag->setMimeData(mime)                                  ;
  dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction) ;
}

void N::VcfPicture::Store(void)
{
  PictureManager PM              ( plan ) ;
  SUID puid    = 0                        ;
  bool success = false                    ;
  success = PM . Import ( Image , puid )  ;
  if ( puid > 0 ) uuid = puid             ;
  emit Store ( this )                     ;
}

typedef struct            {
  bool   overlay          ;
  double x                ;
  double y                ;
  double z                ;
  double left             ;
  double top              ;
  double width            ;
  double height           ;
  double opacity          ;
  double rotation         ;
  double dpi              ;
  double m11              ;
  double m12              ;
  double m13              ;
  double m21              ;
  double m22              ;
  double m23              ;
  double m31              ;
  double m32              ;
  double m33              ;
} VcfPictureConfiguration ;

QByteArray N::VcfPicture::Configuration(void)
{
  QByteArray configuration                                          ;
  configuration.resize(sizeof(VcfPictureConfiguration))             ;
  VcfPictureConfiguration VPC                                       ;
  VPC.overlay  = Overlay                                            ;
  VPC.x        = PaperPos.x             ()                          ;
  VPC.y        = PaperPos.y             ()                          ;
  VPC.z        = zValue                 ()                          ;
  VPC.left     = ScreenRect.left        ()                          ;
  VPC.top      = ScreenRect.top         ()                          ;
  VPC.width    = ScreenRect.width       ()                          ;
  VPC.height   = ScreenRect.height      ()                          ;
  VPC.opacity  = QGraphicsItem::opacity ()                          ;
  VPC.rotation = Angle                                              ;
  VPC.dpi      = PictureDPI                                         ;
  VPC.m11      = Transform.m11          ()                          ;
  VPC.m12      = Transform.m12          ()                          ;
  VPC.m13      = Transform.m13          ()                          ;
  VPC.m21      = Transform.m21          ()                          ;
  VPC.m22      = Transform.m22          ()                          ;
  VPC.m23      = Transform.m23          ()                          ;
  VPC.m31      = Transform.m31          ()                          ;
  VPC.m32      = Transform.m32          ()                          ;
  VPC.m33      = Transform.m33          ()                          ;
  memcpy(configuration.data(),&VPC,sizeof(VcfPictureConfiguration)) ;
  return configuration                                              ;
}

bool N::VcfPicture::setConfiguration(QByteArray & configuration)
{
  if (IsNull(Options))                                       return false ;
  if (configuration.size()!=sizeof(VcfPictureConfiguration)) return false ;
  VcfPictureConfiguration * VPC = (VcfPictureConfiguration *)configuration.data();
  Overlay = VPC->overlay                                   ;
  PaperPos.setX(VPC->x)                                    ;
  PaperPos.setY(VPC->y)                                    ;
  QGraphicsItem::setPos(Options->position(PaperPos))       ;
  setZValue  (VPC->z      )                                ;
  setOpacity (VPC->opacity)                                ;
  ScreenRect . setLeft   (VPC->left  )                     ;
  ScreenRect . setTop    (VPC->top   )                     ;
  ScreenRect . setWidth  (VPC->width )                     ;
  ScreenRect . setHeight (VPC->height)                     ;
  Angle      = VPC->rotation                               ;
  PictureDPI = VPC->dpi                                    ;
  Transform.setMatrix                                      (
    VPC->m11,VPC->m12,VPC->m13                             ,
    VPC->m21,VPC->m22,VPC->m23                             ,
    VPC->m31,VPC->m32,VPC->m33                           ) ;
  QTransform T = Transform                                 ;
  T = T.rotate(Angle)                                      ;
  setTransform(T)                                          ;
  QRectF SR = mapToScene (ScreenRect ).boundingRect()      ;
  PaperRect = Options -> Standard (SR)                     ;
  setToolTip                                               (
    tr("%1 x %2 pixels\n"
       "%3 DPI\n"
       "%4 x %5 cm\n"
       "Center : %6 x %7 cm"
       )
    .arg(Image    .width()).arg(Image    .height())
    .arg(PictureDPI                               )
    .arg(PaperRect.width()).arg(PaperRect.height())
    .arg(PaperPos .x    ()).arg(PaperPos .y     ())
  )                                                        ;
  prepareGeometryChange  (           )                     ;
  return true;
}

void N::VcfPicture::AdjustContrast(void)
{
  QSlider              * slider = new QSlider(Qt::Horizontal);
  QGraphicsProxyWidget * proxy  = new QGraphicsProxyWidget(this);
  proxy->setWidget(slider);
  Proxys  [5] = proxy       ;
  Widgets [5] = slider      ;
  QRectF R = PanelRect()    ;
  proxy->setGeometry(R)     ;
  proxy->setZValue  (0.90f) ;
  proxy->setOpacity (0.50f) ;
  slider->setRange(-255,255);
  slider->setValue(0);
  Original = Image ;
  connect(slider,SIGNAL(valueChanged(int)),this,SLOT(contrastChanged(int)));
}

void N::VcfPicture::contrastChanged(int contrast)
{
  PictureManager PM ( plan )                   ;
  Image = PM.AdjustContrast(Original,contrast) ;
  update ( )                                   ;
}

void N::VcfPicture::UnsharpMask(void)
{
  QSlider              * slider = new QSlider(Qt::Horizontal);
  QGraphicsProxyWidget * proxy  = new QGraphicsProxyWidget(this);
  proxy->setWidget(slider);
  Proxys  [6] = proxy       ;
  Widgets [6] = slider      ;
  QRectF R = PanelRect()    ;
  proxy->setGeometry(R)     ;
  proxy->setZValue  (0.90f) ;
  proxy->setOpacity (0.50f) ;
  slider->setRange(-255,255);
  slider->setValue(0);
  Original = Image ;
  connect(slider,SIGNAL(valueChanged(int)),
          this,SLOT(usmChanged(int)));
}

void N::VcfPicture::usmChanged(int contrast)
{
  PictureManager PM ( plan )                         ;
  Image = PM.UnsharpMask(Original,20,3,0.0,contrast) ;
  update ( )                                         ;
}

void N::VcfPicture::SaveAs(void)
{
  QString filename = QFileDialog::getSaveFileName (
                       GraphicsView()             ,
                       QString::number(uuid)      ,
                       plan->Path("Images")       ,
                       "*.png *.jpg"            ) ;
  if (filename.length()<=0) return                ;
  Image.save(filename)                            ;
  Alert ( Done )                                  ;
}

void N::VcfPicture::JoinColorGroup(void)
{
  SUID            uu = 0                           ;
  UUIDs           Uuids                            ;
  Colors          Colors                           ;
  GraphicsManager GM ( plan )                      ;
  EnterSQL(SC,plan->sql)                           ;
    Uuids = SC.Uuids                               (
              PlanTable(ColorGroups)               ,
              "uuid"                               ,
              SC.OrderByAsc("id")                ) ;
  LeaveSQL(SC,plan->sql)                           ;
  //////////////////////////////////////////////////
  if (Uuids.count()<=0)                            {
    Alert ( Error )                                ;
    return                                         ;
  }                                                ;
  //////////////////////////////////////////////////
  UuidSelection * NUS                              ;
  NUS = new UuidSelection(GraphicsView(),plan)     ;
  NUS->setWindowTitle(tr("Join color group"))      ;
  NUS->setUuids(Uuids)                             ;
  if (NUS->exec()==QDialog::Accepted)              {
    uu = NUS->currentUuid()                        ;
  }                                                ;
  NUS->deleteLater()                               ;
  nDropOut ( uu <= 0 )                             ;
  //////////////////////////////////////////////////
  EnterSQL(XC,plan->sql)                           ;
    GM        . LoadColors ( XC,uu , Colors )      ;
    Graphics :: toColors   ( Image , Colors )      ;
    GM        . SaveColors ( XC,uu , Colors )      ;
  LeaveSQL(XC,plan->sql)                           ;
  //////////////////////////////////////////////////
  Alert ( Done )                                   ;
}
