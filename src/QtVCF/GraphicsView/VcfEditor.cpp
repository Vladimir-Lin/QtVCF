#include <qtvcf.h>

//#include "libav.hpp"
//#include "SDL.h"

#define TABLES(ID) plan->Tables[Tables::ID]

#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

typedef struct                        {
  int                Channel          ; // Channel ID
  int                Type             ; // 0 - Output , 1 - Input
  int                Answer           ;
  int                Channels         ;
  int                SampleFormat     ;
  int                SampleRate       ;
  int                Milliseconds     ;
  int                BytesPerBuffer   ;
  unsigned long      FramesPerBuffer  ; // Frames per buffer
  bool               Fed              ;
  QByteArray         Buffer           ;
  QByteArray         Empty            ;
  nPlan            * Plan             ;
  VcfEditor        * Multimedia       ;
  PaStream         * stream           ;
  PaStreamParameters inputParameters  ;
  PaStreamParameters outputParameters ;
} mmaAudio                            ;

typedef struct                {
    int          Channel      ;
    int          Type         ;
    int          SampleFormat ;
    int          BufferFrames ;
    int          Current      ;
    int          AudioSize    ;
    nAudioData * Data         ;
} mmaFeeder                   ;

#define MMA(id) ((mmaAudio  *)AudioChannels[id])
#define MMF(id) ((mmaFeeder *)AudioFeeders [id])

#ifdef LINUX
typedef struct {
  pthread_t      Thread              ;
  int            Priority            ;
  int            Status              ;
  int            running             ;
  void         * (*func)(void * arg) ;
  nPlan        * Plan                ;
  nMultimedia  * Multimedia          ;
} mmaStream                          ;
#endif

#ifdef MACOSX
typedef struct {
  pthread_t      Thread              ;
  int            Priority            ;
  int            Status              ;
  int            running             ;
  void         * (*func)(void * arg) ;
  nPlan        * Plan                ;
  nMultimedia  * Multimedia          ;
} mmaStream                          ;
#endif

#ifdef WIN32
typedef struct                             {
  HANDLE               ReadThread          ;
  HANDLE               DisplayThread       ;
  HANDLE               AudioThread         ;
  HANDLE               TimeThread          ;

  int                  Priority            ;
  int                  Status              ;
  int                  running             ;
  DWORD                (*func)(LPVOID arg) ;
  DWORD                dwThreadID          ;

  int                  Stream              ;
  int                  Type                ;
  SUID                 Uuid                ;
  QString              filename            ;

  nPlan             *  Plan                ;
  VcfEditor         *  Multimedia          ;

  bool                 isAudio             ;
  bool                 isVideo             ;

  AVFormatContext   *  pFormatCtx          ;
  AVCodecContext    *  vCodecCtx           ;
  AVCodecContext    *  aCodecCtx           ;
  AVStream          *  vStream             ;
  AVStream          *  aStream             ;
  AVCodec           *  vCodec              ;
  AVCodec           *  aCodec              ;
  AVFrame           *  pFrame              ;

  struct SwsContext *  ConvertCtx          ;

  int                  audioStream         ;
  int                  videoStream         ;
  int                  titleStream         ;

  int                  BytesPerSecond      ;
  int                  BytesPerSample      ;
  int                  SampleBits          ;
  int                  PlayBits            ;
  bool                 Float               ;
  int                  AudioBufferSize     ;
  int                  AudioLimit          ;
  int                  AudioSize           ;
  int                  AudioIndex          ;
  long long            AudioBytes          ;
  long long            ActualBytes         ;
  int                  DecodeLength        ;
  unsigned char     *  DecodeBuffer        ;
  unsigned char     *  AudioBuffer         ;
  bool                 Streaming           ;
  ReSampleContext   *  ResampleCtx         ;
  unsigned char     *  ResampleBuffer      ;
  int                  DstPerSample        ;
  int                  SampleSize          ;
  int                  SampleBucket        ;

  QString              Title               ;
  QDateTime            VideoFresh          ;
  QSize                Dimension           ;
  long long            Duration            ;

  void              *  Display             ;

  unsigned char     *  data     [4]        ;
  int                  linesize [4]        ;

  QList<AVPacket *>    Packets             ;
  QList<AVPacket *>    Videos              ;
  QList<AVPacket *>    Audios              ;

} mmaStream                                ;
#endif

#define MMS(id) ((mmaStream *)StreamChannels [id])

typedef struct         {
  QWidget     * Widget ;
  SDL_Surface * Screen ;
  SDL_Overlay * YUV    ;
  SDL_Rect      Rect   ;
} DisplaySDL           ;

#define MMD(id) ((DisplaySDL *)DisplayChannels [id])

static int multimediaCallback(
             const void                     * inputBuffer     ,
             void                           * outputBuffer    ,
             unsigned long                    framesPerBuffer ,
             const PaStreamCallbackTimeInfo * timeInfo        ,
                   PaStreamCallbackFlags      statusFlags     ,
             void                           * userData        )
{
  mmaAudio * mmaa = (mmaAudio *)userData               ;
  nKickOut ( IsNull ( mmaa            ) , paContinue ) ;
  nKickOut ( IsNull ( mmaa->Multimedia) , paContinue ) ;
  mmaa->Multimedia->FeedAudio                          (
      mmaa->Channel , outputBuffer , framesPerBuffer ) ;
  int answer = mmaa->Answer                            ;
  return answer                                        ;
}

static int multimediaRecorder                                 (
             const void                     * inputBuffer     ,
             void                           * outputBuffer    ,
             unsigned long                    framesPerBuffer ,
             const PaStreamCallbackTimeInfo * timeInfo        ,
             PaStreamCallbackFlags            statusFlags     ,
             void                           * userData        )
{
  mmaAudio * mmaa = (mmaAudio *)userData              ;
  nKickOut ( IsNull ( mmaa            ) , paContinue ) ;
  nKickOut ( IsNull ( mmaa->Multimedia) , paContinue ) ;
  mmaa->Multimedia->RecordAudio                       (
      mmaa->Channel , inputBuffer , framesPerBuffer ) ;
  return paContinue                                   ;
}

#ifdef LINUX
static void * streamCallback(void * arg)
{
  mmaStream * mmas = (mmaStream*)arg               ;
  if (IsNull(mmas)) return NULL                    ;
  mmas->Multimedia->RunStream     ( mmas->Stream ) ;
  mmas->Multimedia->DestroyStream ( mmas->Stream ) ;
  pthread_exit(arg)                                ;
  return NULL                                      ;
}
#endif

#ifdef MACOSX
static void * streamCallback(void * arg)
{
  mmaStream * mmas = (mmaStream*)arg               ;
  if (IsNull(mmas)) return NULL                    ;
  mmas->Multimedia->RunStream     ( mmas->Stream ) ;
  mmas->Multimedia->DestroyStream ( mmas->Stream ) ;
  pthread_exit(arg)                                ;
  return NULL                                      ;
}
#endif

#ifdef WIN32
static DWORD streamCallback(LPVOID arg)
{
  mmaStream * mmas = (mmaStream*)arg               ;
  nKickOut ( IsNull ( mmas ) , 0 )                 ;
  mmas->Multimedia->RunStream     ( mmas->Stream ) ;
  mmas->Multimedia->DestroyStream ( mmas->Stream ) ;
  return 0                                         ;
}
#endif

#endif

//////////////////////////////////////////////////////////////////////////////

N::VcfEditor:: VcfEditor      (QWidget * parent,Plan * p)
             : GraphicsView   (          parent,       p)
             , Mode           (FreeMode                 )
             , Method         (0                        )
             , Group          (0                        )
             , Uuid           (0                        )
             , SourceType     (Types::None              )
             , Version        (""                       )
             , Title          (""                       )
{
  Configure ( ) ;
}

N::VcfEditor::~VcfEditor(void)
{
}

void N::VcfEditor::drawBackground (QPainter * painter,const QRectF & rect)
{
  QGraphicsView :: drawBackground ( painter , rect ) ;
}

void N::VcfEditor::drawForeground (QPainter * painter,const QRectF & rect)
{
  QGraphicsView :: drawForeground ( painter , rect ) ;
}

void N::VcfEditor::enterEvent(QEvent * event)
{
  QGraphicsView :: enterEvent ( event ) ;
  Alert ( Action )                      ;
}

void N::VcfEditor::leaveEvent(QEvent * event)
{
  QGraphicsView :: leaveEvent ( event ) ;
  Alert ( Action )                      ;
}

void N::VcfEditor::paintEvent(QPaintEvent * event)
{
  nIsolatePainter(QGraphicsView) ;
}

void N::VcfEditor::dragEnterEvent(QDragEnterEvent * event)
{
  if (dragEnter(event))                    {
    event->acceptProposedAction()          ;
  } else                                   {
    if (PassDragDrop)                      {
      QGraphicsView::dragEnterEvent(event) ;
    } else event->ignore()                 ;
  }                                        ;
}

void N::VcfEditor::dragLeaveEvent(QDragLeaveEvent * event)
{
  if (removeDrop()) event->accept() ; else {
    if (PassDragDrop)                      {
      QGraphicsView::dragLeaveEvent(event) ;
    } else event->ignore()                 ;
  }                                        ;
}

void N::VcfEditor::dragMoveEvent(QDragMoveEvent * event)
{
  if (dragMove(event))                    {
    event->acceptProposedAction()         ;
  } else                                  {
    if (PassDragDrop)                     {
      QGraphicsView::dragMoveEvent(event) ;
    } else event->ignore()                ;
  }                                       ;
}

void N::VcfEditor::dropEvent(QDropEvent * event)
{
  if (drop(event))                    {
    event->acceptProposedAction()     ;
  } else                              {
    if (PassDragDrop)                 {
      QGraphicsView::dropEvent(event) ;
    } else event->ignore()            ;
  }                                   ;
}

bool N::VcfEditor::focusNextPrevChild(bool next)
{
  return QGraphicsView::focusNextPrevChild(next);
}

void N::VcfEditor::focusInEvent(QFocusEvent * event)
{
  if (!focusIn (event)) QGraphicsView::focusInEvent (event) ;
}

void N::VcfEditor::focusOutEvent(QFocusEvent * event)
{
  if (!focusOut(event)) QGraphicsView::focusOutEvent(event) ;
}

void N::VcfEditor::inputMethodEvent(QInputMethodEvent * event)
{
  QGraphicsView::inputMethodEvent(event);
}

void N::VcfEditor::keyPressEvent(QKeyEvent * event)
{
  QGraphicsView::keyPressEvent(event);
}

void N::VcfEditor::keyReleaseEvent(QKeyEvent * event)
{
  QGraphicsView::keyReleaseEvent(event);
}

void N::VcfEditor::contextMenuEvent(QContextMenuEvent * event)
{
  if (Menu(event->pos())) event->accept() ; else
  QGraphicsView::contextMenuEvent(event)  ;
}

void N::VcfEditor::mousePressEvent(QMouseEvent * event)
{
  QGraphicsView::mousePressEvent(event);
}

void N::VcfEditor::mouseReleaseEvent(QMouseEvent * event)
{
  QGraphicsView::mouseReleaseEvent(event);
}

void N::VcfEditor::mouseMoveEvent(QMouseEvent * event)
{
  QGraphicsView::mouseMoveEvent(event);
}

void N::VcfEditor::mouseDoubleClickEvent(QMouseEvent * event)
{
  QGraphicsView::mouseDoubleClickEvent(event);
}

void N::VcfEditor::wheelEvent(QWheelEvent * event)
{
  QGraphicsView::wheelEvent(event);
}

void N::VcfEditor::resizeEvent(QResizeEvent * event)
{
  if (Relocation()) event->accept (     ) ;
  else QGraphicsView::resizeEvent (event) ;
}

bool N::VcfEditor::event(QEvent * event)
{
  if (nEqual(event->type(),QEvent::KeyPress))        {
    QKeyEvent * ke = static_cast<QKeyEvent *>(event) ;
    if (NotNull(ke))                                 {
      if (nEqual(ke->key(),Qt::Key_Tab)) return true ; // intercepts Tab key
    }                                                ;
  }                                                  ;
  return GraphicsView :: event ( event )             ;
}

bool N::VcfEditor::isRunning(void)
{
  return false ;
}

bool N::VcfEditor::Interrupt(void)
{
  return false ;
}

void N::VcfEditor::Configure(void)
{
  screen = plan->screen                   ;
  setRelocation  (PerfectRegion   )       ;
  setWindowTitle (tr("VCF Editor"))       ;
  setScene       (Scene           )       ;
  setTransform   (Transform       )       ;
  DefaultZooms   (                )       ;
  setRelocation  (PaperRegion     )       ;
  ConnectScrolls (                )       ;
  /////////////////////////////////////////
  QLabel * label                          ;
  /////////////////////////////////////////
  label = new QLabel()                    ;
  plan->status->addPermanentWidget(label) ;
  Widgets[vePage  ] = label               ;
  label->hide()                           ;
  /////////////////////////////////////////
  label = new QLabel()                    ;
  plan->status->addPermanentWidget(label) ;
  Widgets[veCursor] = label               ;
  label->hide()                           ;
}

bool N::VcfEditor::FocusIn(void)
{
  nKickOut     ( IsNull(plan) , false                                   ) ;
  connectAction(Menus::ZoomIn    ,this,SLOT(ZoomIn    ()),true) ;
  connectAction(Menus::ZoomOut   ,this,SLOT(ZoomOut   ()),true) ;
  connectAction(Menus::Print     ,this,SLOT(Print     ()),true) ;
  connectAction(Menus::Export    ,this,SLOT(Export    ()),this) ;
  connectAction(Menus::ExportPDF ,this,SLOT(ExportPDF ()),this) ;
  connectAction(Menus::Load      ,this,SLOT(Load      ()),this) ;
  connectAction(Menus::Save      ,this,SLOT(Save      ()),this) ;
  connectAction(Menus::SaveAs    ,this,SLOT(SaveAs    ()),this) ;
  connectAction(Menus::Close     ,this,SLOT(Close     ()),this) ;
  connectAction(Menus::Undo      ,this,SLOT(Undo      ()),this) ;
  connectAction(Menus::Redo      ,this,SLOT(Redo      ()),this) ;
  connectAction(Menus::Select    ,this,SLOT(Select    ()),this) ;
  connectAction(Menus::Properties,this,SLOT(Properties()),this) ;
  Alert        ( Click                                                  ) ;
  /////////////////////////////////////////////////////////////////////////
  QLabel * label                                                          ;
  /////////////////////////////////////////////////////////////////////////
  label = GetLabel ( veCursor )                                           ;
  if (NotNull(label)) label -> setVisible ( true  )                       ;
  label = GetLabel ( vePage   )                                           ;
  if (NotNull(label)) label -> setVisible ( true  )                       ;
  return true                                                             ;
}

bool N::VcfEditor::FocusOut(void)
{
  QLabel * label                                    ;
  ///////////////////////////////////////////////////
  label = GetLabel ( veCursor )                     ;
  if (NotNull(label)) label -> setVisible ( false ) ;
  label = GetLabel ( vePage   )                     ;
  if (NotNull(label)) label -> setVisible ( false ) ;
  return true                                       ;
}

void N::VcfEditor::PerfectView(void)
{
  View = asPaper (available(size())) ;
  setSceneRect   (View             ) ;
  setTransform   (Transform        ) ;
}

bool N::VcfEditor::Relocation(void)
{
  nKickOut (IsNull(plan),false) ;
  QWidget * vp = viewport()     ;
  nKickOut (IsNull(vp  ),false) ;
  Zoom ()                       ;
  return false                  ;
}

bool N::VcfEditor::Shutdown(void)
{
  QLabel * label                               ;
  //////////////////////////////////////////////
  label = GetLabel ( veCursor )                ;
  if (NotNull(label)) label -> deleteLater ( ) ;
  label = GetLabel ( vePage   )                ;
  if (NotNull(label)) label -> deleteLater ( ) ;
  //////////////////////////////////////////////
//  Save ( )                                     ;
  return true                                  ;
}

void N::VcfEditor::DefaultZooms(void)
{
  setDefaultZoom ( ) ;
}

void N::VcfEditor::ZoomIn(void)
{
  Enlarge (       ) ;
  Zoom    (       ) ;
  Alert   ( Click ) ;
}

void N::VcfEditor::ZoomOut(void)
{
  Shrink (       ) ;
  Zoom   (       ) ;
  Alert  ( Click ) ;
}

void N::VcfEditor::UnitedZoom(void)
{
  QRectF V = asPaper (available(size())) ;
  View     = United  (this,V           ) ;
  setSceneRect       (View             ) ;
  setTransform       (Transform        ) ;
}

void N::VcfEditor::PaperZoom(void)
{
  QRectF V = asPaper (available(size()))   ;
  if (range.width()>0 && range.height()>0) {
    View = range                           ;
  } else View = V                          ;
  setSceneRect       (View             )   ;
  setTransform       (Transform        )   ;
}

void N::VcfEditor::AddPaper(void)
{
  VcfPaperManager::AddPaper                              (
    this , Scene , &Options                              ,
    SLOT(PaperMoving(QString,QPointF,QPointF,QPointF)) ) ;
  Zoom()                                                 ;
}

void N::VcfEditor::Zoom(void)
{
  switch (RelocationMode)                  {
    nFastCast(PerfectRegion,PerfectView()) ;
    nFastCast(UnitedRegion ,UnitedZoom ()) ;
    nFastCast(PaperRegion  ,PaperZoom  ()) ;
  }                                        ;
}

N::VcfCanvas * N::VcfEditor::NewCanvas(int mode,VcfItem * vcf)
{
  VcfCanvas * vc = NewCanvas(vcf) ;
  vc -> Mode = mode               ;
  return vc                       ;
}

N::VcfCanvas * N::VcfEditor::NewCanvas(VcfItem * vcf)
{
  VcfCanvas * vc = new VcfCanvas(this,vcf)                 ;
  addItem ( vc , vcf )                                     ;
  scene()->addItem ( vc )                                     ;
  vc->plan    = plan                                       ;
  vc->Options = &Options                                   ;
  connect(vc  ,SIGNAL(Menu          (VcfCanvas*,QPointF))  ,
          this,SLOT  (CanvasMenu    (VcfCanvas*,QPointF))) ;
  connect(vc  ,SIGNAL(ShapeName(VcfCanvas*,QString))       ,
          this,SLOT  (ShapeName(VcfCanvas*,QString))     ) ;
  return vc                                                ;
}

N::VcfPaper * N::VcfEditor::NewPaper(VcfItem * vcf)
{
  VcfPaper * vp = new VcfPaper(this,vcf)     ;
  addItem ( vp , vcf )                       ;
  scene()->addItem ( vp )                       ;
  vp->plan    = plan                         ;
  vp->Options = &Options                     ;
  return vp                                  ;
}

N::VcfPicture * N::VcfEditor::NewPicture(VcfItem * vcf)
{
  VcfPicture * vp = new VcfPicture(this,vcf)                     ;
  addItem ( vp , vcf )                                           ;
  scene()->addItem ( vp )                                           ;
  vp->plan    = plan                                             ;
  vp->Options = &Options                                         ;
  connect(vp  ,SIGNAL(Selection          (VcfItem*,QRectF))      ,
          this,SLOT  (Selection          (VcfItem*,QRectF))    ) ;
  connect(vp  ,SIGNAL(Canvas             (VcfItem*,QRectF))      ,
          this,SLOT  (Canvas             (VcfItem*,QRectF))    ) ;
  connect(vp  ,SIGNAL(Delete             (VcfPicture*))          ,
          this,SLOT  (DeletePicture      (VcfPicture*))        ) ;
  connect(vp  ,SIGNAL(Store              (VcfPicture*))          ,
          this,SLOT  (StorePicture       (VcfPicture*))        ) ;
  connect(vp  ,SIGNAL(Menu               (VcfPicture*,QPointF))  ,
          this,SLOT  (PictureMenu        (VcfPicture*,QPointF))) ;
  connect(vp  ,SIGNAL(Process            (VcfPicture*))          ,
          this,SLOT  (PictureProcess     (VcfPicture*))        ) ;
  connect(vp  ,SIGNAL(Faces              (VcfPicture*))          ,
          this,SLOT  (PictureFaces       (VcfPicture*))        ) ;
  connect(vp  ,SIGNAL(LoadSelections     (VcfPicture*))          ,
          this,SLOT  (PictureSelections  (VcfPicture*))        ) ;
  return vp                                                      ;
}

N::VcfPicture * N::VcfEditor::NewPicture(VcfItem * vcf,SUID uuid)
{
  VcfPicture * vp = NewPicture(vcf) ;
  vp   -> LoadImage ( uuid )        ;
  return vp                         ;
}

N::VcfPicture * N::VcfEditor::NewPicture(VcfItem * vcf,QImage image)
{
  VcfPicture * vp = NewPicture(vcf) ;
  vp   -> Image   = image           ;
  return vp                         ;
}

N::VcfLabel * N::VcfEditor::NewLabel(VcfItem * vcf,QRectF paper,QString content)
{
  VcfLabel * label = new VcfLabel(this,vcf)              ;
  label->Content = content                               ;
  addItem ( label , vcf )                                ;
  scene()->addItem ( label )                                ;
  Font FONT(plan->fonts[Fonts::Default])                 ;
  FONT.cm = 0.48                                         ;
  label->plan    = plan                                  ;
  label->Options = &Options                              ;
  label->Painter.fonts [0] = FONT                        ;
  label->setRange ( paper )                              ;
  connect(label,SIGNAL(Selection     (VcfItem*,QRectF))  ,
          this ,SLOT  (Selection     (VcfItem*,QRectF))) ;
  connect(label,SIGNAL(Canvas        (VcfItem*,QRectF))  ,
          this ,SLOT  (Canvas        (VcfItem*,QRectF))) ;
  return label                                           ;
}

N::VcfColorPicker * N::VcfEditor::NewColorPicker(VcfItem * vcf)
{
  VcfColorPicker * vcp = new VcfColorPicker(this,NULL) ;
  addItem ( vcp , vcf )                                ;
  scene()->addItem ( vcp )                                ;
  connect(vcp ,SIGNAL(RemoveItem(VcfItem*))            ,
          this,SLOT  (Remove    (VcfItem*))          ) ;
  connect(vcp ,SIGNAL(Append(VcfItem*,VcfItem*))       ,
          this,SLOT  (Append(VcfItem*,VcfItem*))     ) ;
  vcp -> Mode    = VcfCanvas::Board                    ;
  vcp -> plan    = plan                                ;
  vcp -> Options = &Options                            ;
  return vcp                                           ;
}

N::VcfInterface * N::VcfEditor::NewInterface(VcfItem * vcf)
{
  VcfInterface * vci = new VcfInterface(this,NULL) ;
  addItem ( vci , vcf )                            ;
  scene()->addItem ( vci )                            ;
  connect(vci ,SIGNAL(RemoveItem(VcfItem*))        ,
          this,SLOT  (Remove    (VcfItem*))      ) ;
  connect(vci ,SIGNAL(Append(VcfItem*,VcfItem*))   ,
          this,SLOT  (Append(VcfItem*,VcfItem*)) ) ;
  vci -> Mode    = VcfCanvas::Board                ;
  vci -> plan    = plan                            ;
  vci -> Options = &Options                        ;
  return vci                                       ;
}

void N::VcfEditor::PutPicture(VcfPicture * picture,double zValue)
{
  picture -> setPos     ( QPointF ( 0.0 , 0.0  ) ) ;
  picture -> setZValue  ( zValue                 ) ;
  picture -> setOpacity ( 1.0                    ) ;
  picture -> setCenter  ( QPointF ( 0.0 , 0.0  ) ) ;
  picture -> setPos     ( picture->PaperMiddle() ) ;
}

void N::VcfEditor::ShowPicture(VcfPicture * picture)
{
  QRect R = toView(this,picture)                        ;
  setGeometry (QRect(x(),y(),R.width()+8,R.height()+8)) ;
}

void N::VcfEditor::SolitaryPicture(VcfPicture * picture,double zValue)
{
  Zoom        (                  ) ;
  PutPicture  ( picture , zValue ) ;
  FocusIn     (                  ) ;
  ShowPicture ( picture          ) ;
  Zoom        (                  ) ;
}

bool N::VcfEditor::validPicture(QFileInfo info)
{
  QImage Image                                           ;
  if (!Image.load(info.absoluteFilePath())) return false ;
  return true                                            ;
}

void N::VcfEditor::AdjustMdi(QMdiArea * mdi)
{
  QRect V = mapFromScene ( View ) . boundingRect ()                        ;
  QMdiSubWindow * w  = Casting(QMdiSubWindow,parent())                     ;
  QWidget       * vp = viewport()                                          ;
  nDropOut ( IsNull(w ) )                                                  ;
  nDropOut ( IsNull(vp) )                                                  ;
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

bool N::VcfEditor::LoadPicture(SUID uuid)
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

bool N::VcfEditor::LoadPicture(QFileInfo info)
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

void N::VcfEditor::ViewPicture(SUID uuid,MdiArea * Mdi)
{
  setWindowTitle      ( tr("Picture View")    ) ;
  setScene            ( Scene                 ) ;
  setTransform        ( Transform             ) ;
  DefaultZooms        (                       ) ;
  setRelocation       ( UnitedRegion          ) ;
  setScrollBarPolicy  ( Qt::ScrollBarAsNeeded ) ;
  LoadPicture         ( uuid                  ) ;
  Mdi -> Fit          ( this                  ) ;
  show                (                       ) ;
  DoProcessEvents                               ;
  AdjustMdi           ( Mdi                   ) ;
}

void N::VcfEditor::ViewPicture(QFileInfo info,MdiArea * Mdi)
{
  setWindowTitle      ( tr("Picture View")    ) ;
  setScene            ( Scene                 ) ;
  setTransform        ( Transform             ) ;
  DefaultZooms        (                       ) ;
  setRelocation       ( UnitedRegion          ) ;
  setScrollBarPolicy  ( Qt::ScrollBarAsNeeded ) ;
  LoadPicture         ( info                  ) ;
  Mdi -> Fit          ( this                  ) ;
  show                (                       ) ;
  DoProcessEvents                               ;
  AdjustMdi           ( Mdi                   ) ;
}

void N::VcfEditor::DeletePicture(VcfPicture * picture)
{
  Remove ( picture ) ;
}

void N::VcfEditor::StorePicture(VcfPicture * picture)
{
  emit Store ( Method , Group , picture->uuid ) ;
}

void N::VcfEditor::PictureMenu(VcfPicture * picture,QPointF pos)
{
  QPoint global = picture -> toGlobal ( pos ) ;
  picture -> showMenu ( this , global )       ;
}

void N::VcfEditor::PictureProcess(VcfPicture * picture)
{
}

void N::VcfEditor::PictureFaces(VcfPicture * picture)
{
  PictureManager    PM ( plan )                               ;
  PictureRecognizer PR ( plan )                               ;
  IplImage * Image                                            ;
  Image = PM . toIplImage ( picture->Image )                  ;
  nDropOut ( IsNull(Image) )                                  ;
  Phiz faces = PR . Faces ( Image )                           ;
  Alert ( Done )                                              ;
  nDropOut ( faces . count() <= 0 )                           ;
  QSize  I = picture -> Image . size ()                       ;
  QRectF S = picture -> ScreenRect                            ;
  double Z = picture -> zValue()                              ;
  Z += 0.1 ; if (Z>1.0) Z = 1.0                               ;
  for (int i=0;i<faces.count();i++)                           {
    QRect   Face                                              ;
    QRectF iFace                                              ;
    Face  = faces[i].Face                                     ;
    Face  = PR . FlatRatio ( Face , I )                       ;
    iFace = PR . ShiftIn   ( S , Face )                       ;
    VcfCanvas * canvas = NewCanvas ( picture )                ;
    canvas -> Mode = VcfCanvas::Board                         ;
    canvas -> setCanvas  ( iFace      )                       ;
    canvas -> setZValue  ( Z          )                       ;
    canvas -> setOpacity ( 0.35f      )                       ;
    if (faces[i].isIntact())                                  {
      canvas->Painter.brushes[0].setColor(QColor(192,255,160));
      canvas -> setToolTip                                    (
        tr("Face [%1 x %2]"                                   )
        .arg(Face.width ()                                    )
        .arg(Face.height()                                    )
      )                                                       ;
    } else
    if (faces[i].isPartial())                                 {
      int possibility = (int)( faces[i].Possibility() * 100 ) ;
      QString msg = tr("Possible %1% face [%2 x %3]"          )
      .arg(possibility                                        )
      .arg(Face.width ()                                      )
      .arg(Face.height()                                    ) ;
      if (faces[i].LeftEye .count()>0)                        {
        msg += "\n"                                           ;
        msg += tr("Possible left eye detected")               ;
      }                                                       ;
      if (faces[i].RightEye.count()>0)                        {
        msg += "\n"                                           ;
        msg += tr("Possible right eye detected")              ;
      }                                                       ;
      if (faces[i].Mouth   .count()>0)                        {
        msg += "\n"                                           ;
        msg += tr("Possible mouth detected")                  ;
      }                                                       ;
      if (faces[i].Noses   .count()>0)                        {
        msg += "\n"                                           ;
        msg += tr("Possible nose detected")                   ;
      }                                                       ;
      canvas->Painter.brushes[0].setColor(QColor(255,160,192));
      canvas -> setToolTip ( msg )                            ;
    }                                                         ;
  }                                                           ;
}

void N::VcfEditor::PictureSelections(VcfPicture * picture)
{
  nDropOut ( IsNull(picture) )                        ;
  SUID uuid = picture->uuid                           ;
  nDropOut ( uuid <= 0       )                        ;
  QMap<SUID,QRect> Faces                              ;
  UUIDs            Uuids                              ;
  Sql sql = plan->sql                                 ;
  SqlConnection SC(sql)                               ;
  if (SC.open("VcfEditor","PictureSelections"))       {
    QString Q                                         ;
    SUID    xuid                                      ;
    Q = sql.SelectFrom                                (
          "second"                                    ,
          PlanTable(Groups)                           ,
          QString("where first = %1 and t1 = %2 and t2 = %3 and relation = %4 order by position asc")
            .arg(uuid                                 )
            .arg(Types::Picture                       )
            .arg(Types::Rectangle                     )
            .arg(Groups::Subordination                )
        )                                             ;
    SqlLoopNow ( SC , Q )                             ;
      xuid = SC . Uuid ( 0 )                          ;
      Uuids << xuid                                   ;
    SqlLoopErr ( SC , Q )                             ;
    SqlLoopEnd ( SC , Q )                             ;
    foreach (xuid,Uuids)                              {
      Q = sql.SelectFrom                              (
            "x,y,w,h"                                 ,
            PlanTable(Rectangles)                     ,
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

N::VcfPicture * N::VcfEditor::CropPicture(VcfPicture * picture,VcfCanvas * canvas)
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

N::VcfPicture * N::VcfEditor::CannyPicture(VcfPicture * picture,VcfCanvas * canvas)
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

N::VcfPicture * N::VcfEditor::SobelPicture(VcfPicture * picture,VcfCanvas * canvas)
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

N::VcfPicture * N::VcfEditor::LaplacePicture(VcfPicture * picture,VcfCanvas * canvas)
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

QList<N::VcfContours *> N::VcfEditor::CannyDetection(VcfPicture * picture,VcfCanvas * canvas)
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

QList<N::VcfContours *> N::VcfEditor::SobelDetection(VcfPicture * picture,VcfCanvas * canvas)
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

QList<N::VcfContours *> N::VcfEditor::LaplaceDetection(VcfPicture * picture,VcfCanvas * canvas)
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

QList<N::VcfContours *> N::VcfEditor::ContoursDetection (VcfPicture * picture,VcfCanvas * canvas)
{
  QList<VcfContours *> contours ;
  return contours               ;
}

void N::VcfEditor::SaveSelection(VcfPicture * picture,VcfCanvas * canvas)
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

void N::VcfEditor::RemoveSelection(VcfPicture * picture,VcfCanvas * canvas)
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

void N::VcfEditor::Selection(VcfItem * item,QRectF Screen)
{
  VcfCanvas * canvas = NewCanvas ( item )      ;
  canvas -> Mode = VcfCanvas::Border           ;
  canvas -> setCanvas  ( Screen              ) ;
  canvas -> setZValue  ( item->zValue()+0.01 ) ;
  canvas -> setOpacity ( 0.50f               ) ;
}

void N::VcfEditor::Canvas(VcfItem * item,QRectF Screen)
{
  VcfCanvas * canvas = NewCanvas ( item )      ;
  canvas -> Mode = VcfCanvas::Board            ;
  canvas -> setCanvas  ( Screen              ) ;
  canvas -> setZValue  ( item->zValue()+0.01 ) ;
  canvas -> setOpacity ( 0.25f               ) ;
}

void N::VcfEditor::PaperMenu(VcfPaper * paper,QPointF pos)
{
}

void N::VcfEditor::CanvasMenu(VcfCanvas * canvas,QPointF pos)
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
    mp = mm.addMenu(tr("Pictures"                       ))              ;
    mm.add(mp,30001,tr("Crop image within selected area"))              ;
    mm.add(mp,30002,tr("Save selection"                 ))              ;
    mm.addSeparator   (mp                                )              ;
    mm.add(mp,30003,tr("Canny operator"                 ))              ;
    mm.add(mp,30004,tr("Sobel operator"                 ))              ;
    mm.add(mp,30005,tr("Laplace operator"               ))              ;
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
      SobelPicture      ( picture , canvas )                            ;
    break                                                               ;
    case 30005                                                          :
      LaplacePicture    ( picture , canvas )                            ;
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

void N::VcfEditor::ContourMenu(VcfContours * contour,QPointF pos)
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

void N::VcfEditor::ShapeName(VcfCanvas * canvas,QString name)
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

void N::VcfEditor::EditPenColor(VcfCanvas * canvas)
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

void N::VcfEditor::EditBrushColor(VcfCanvas * canvas)
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

void N::VcfEditor::EditGradient(VcfCanvas * canvas)
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

void N::VcfEditor::StoreContours(VcfCanvas * canvas,VcfContours * contour)
{
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  VcfInterface      * vine = NULL                 ;
  ContourController * cc   = NULL                 ;
  vine = new VcfInterface       ( this , canvas ) ;
  cc   = new nContourController ( NULL , Plan   ) ;
  QRect RN = cc->geometry()                       ;
  QRect RF( 0, 0,RN.width()+8,RN.height()+32)     ;
  QRect RC( 4,28,RN.width()  ,RN.height()   )     ;
  QRect HR( 4, 2,RN.width()  ,            24)     ;
  QPointF GP (0.0,0.0)                            ;
  vine -> Plan = Plan                             ;
  vine -> showHeader = true                       ;
  vine -> HeaderRect = HR                         ;
  vine -> Title      = cc->windowTitle()          ;
  vine -> setOptions      ( Options,true       )  ;
  vine -> setZValue       ( 1.00f              )  ;
  vine -> setWidget       ( cc  , GP , RF , RC )  ;
  vine -> adjustTransform (                    )  ;
  cc   -> Canvas  = canvas                        ;
  cc   -> Contour = contour                       ;
  Append ( vine , canvas )                        ;
  connect(vine,SIGNAL(Finish(VcfItem*))           ,
          this,SLOT  (Remove(VcfItem*))         ) ;
  connect(cc  ,SIGNAL(Finish())                   ,
          vine,SLOT  (Finish())                 ) ;
  cc   -> Display ( )                             ;
#endif
}

void N::VcfEditor::EditFlags(QGraphicsItem * item)
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

bool N::VcfEditor::OutputChannel   (
        int Channel             ,
        int DeviceId            ,
        int BufferSize          ,
        int Channels            ,
        int SampleFormat        ,
        int SampleRate          )
{
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  if (!CreateChannel(Channel)) return false                                    ;
  //////////////////////////////////////////////////////////////////////////////
  PaError err  = ::Pa_Initialize ( )                                           ;
  if (err!=paNoError)                                                          {
    ClearChannel ( Channel )                                                   ;
    return false                                                               ;
  }                                                                            ;
  MMA(Channel)->outputParameters.device = DeviceId                             ;
  //////////////////////////////////////////////////////////////////////////////
  if (DeviceId<0 && plan->Equipments.contains(nPlan::DefaultAudioOutput))      {
    const PaDeviceInfo * deviceInfo                                            ;
    QTextCodec         * codec  = QTextCodec :: codecForLocale ( )             ;
    int                  ds     = ::Pa_GetDeviceCount ( )                      ;
    QString              AD     = plan->Equipments[nPlan::DefaultAudioOutput]  ;
    for (int ii=0;ii<ds;ii++)                                                  {
      deviceInfo = ::Pa_GetDeviceInfo(ii)                                      ;
      if (deviceInfo->maxOutputChannels>0)                                     {
        QByteArray B(deviceInfo->name)                                         ;
        if (B.size()>0)                                                        {
          QString Name = codec->toUnicode(deviceInfo->name)                    ;
          if (Name==AD) MMA(Channel)->outputParameters.device = ii             ;
        }                                                                      ;
      }                                                                        ;
    }                                                                          ;
  }                                                                            ;
  //////////////////////////////////////////////////////////////////////////////
  bool failure = false                                                         ;
  if ( MMA(Channel) -> outputParameters . device <  0         ) failure = true ;
  if ( MMA(Channel) -> outputParameters . device == paNoDevice) failure = true ;
  if ( failure )                                                               {
    ClearChannel ( Channel )                                                   ;
    return false                                                               ;
  }                                                                            ;
  //////////////////////////////////////////////////////////////////////////////
  MMA(Channel)->outputParameters.channelCount              = Channels          ;
  MMA(Channel)->outputParameters.suggestedLatency          = ::Pa_GetDeviceInfo(
              MMA(Channel)->outputParameters.device )->defaultLowOutputLatency ;
  MMA(Channel)->outputParameters.hostApiSpecificStreamInfo = NULL              ;
  MMA(Channel)->outputParameters.sampleFormat              = SampleFormat      ;
  MMA(Channel)->Type                                       = 1                 ;
  MMA(Channel)->Channels                                   = Channels          ;
  MMA(Channel)->SampleFormat                               = SampleFormat      ;
  MMA(Channel)->SampleRate                                 = SampleRate        ;
  MMA(Channel)->Answer                                     = paContinue        ;
  //////////////////////////////////////////////////////////////////////////////
  err = ::Pa_OpenStream                                                        (
            &(MMA(Channel)->stream)                                            ,
            NULL                                                               ,
            &(MMA(Channel)->outputParameters)                                  ,
            SampleRate                                                         ,
            BufferSize                                                         ,
            paClipOff                                                          ,
            multimediaCallback                                                 ,
            MMA(Channel)                                                     ) ;
  //////////////////////////////////////////////////////////////////////////////
  failure = false                                                              ;
  if (err!=paNoError) failure = true                                           ;
  if (!failure)                                                                {
    err = ::Pa_StartStream ( MMA(Channel)->stream )                            ;
    if (err!=paNoError) failure = true                                         ;
  }                                                                            ;
  //////////////////////////////////////////////////////////////////////////////
  if ( failure )                                                               {
    ClearChannel ( Channel )                                                   ;
    return false                                                               ;
  }                                                                            ;
#endif
  return true                                                                  ;
}

bool N::VcfEditor::StopOutput(int Channel)
{
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  bool failure = false                                           ;
  PaError err                                                    ;
  if (IsNull(MMA(Channel))                    ) failure = true   ;
  if (!failure && IsNull(MMA(Channel)->stream)) failure = true   ;
  if (!failure)   err = ::Pa_StopStream ( MMA(Channel)->stream ) ;
  if (!failure && err!=paNoError) failure = true                 ;
  if (!failure)   err = ::Pa_CloseStream( MMA(Channel)->stream ) ;
  if (!failure && err!=paNoError) failure = true                 ;
  if (!failure) ::Pa_Terminate()                                 ;
  if (!failure) ClearChannel(Channel)                            ;
#endif
  return true                                                    ;
}

void N::VcfEditor::FeedAudio(int channel,void * output,unsigned long framesPerBuffer)
{
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  if (!AudioChannels.contains(channel)) return          ;
  if (MMA(channel)->FramesPerBuffer<=0)                 {
    OptimizeChannel ( channel , framesPerBuffer )       ;
  } else                                                {
    if (OutputMaps.contains(channel))                   {
      int feeder = OutputMaps[channel]                  ;
      FeedChannel ( channel , feeder , output )         ;
    } else                                              {
      uchar * src = (uchar *)MMA(channel)->Empty.data() ;
      memcpy(output,src,MMA(channel)->BytesPerBuffer)   ;
    }                                                   ;
  }                                                     ;
#endif
}

void N::VcfEditor::OptimizeChannel(int channel,unsigned long framesPerBuffer)
{
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  if (framesPerBuffer<=0) return                                  ;
  switch (MMA(channel)->SampleFormat)                             {
    case paUInt8                                                  :
      MMA(channel)->BytesPerBuffer = 1                            ;
    break                                                         ;
    case paInt16                                                  :
      MMA(channel)->BytesPerBuffer = 2                            ;
    break                                                         ;
    case paInt32                                                  :
      MMA(channel)->BytesPerBuffer = 4                            ;
    break                                                         ;
    case paFloat32                                                :
      MMA(channel)->BytesPerBuffer = 4                            ;
    break                                                         ;
  }                                                               ;
  MMA(channel)->Milliseconds    = 1000                            ;
  MMA(channel)->Milliseconds   *= framesPerBuffer                 ;
  MMA(channel)->Milliseconds   /= MMA(channel)->SampleRate        ;
  MMA(channel)->BytesPerBuffer *= MMA(channel)->Channels          ;
  MMA(channel)->BytesPerBuffer *= framesPerBuffer                 ;
  MMA(channel)->Buffer.resize(MMA(channel)->BytesPerBuffer)       ;
  MMA(channel)->Empty .resize(MMA(channel)->BytesPerBuffer)       ;
  int total = MMA(channel)->Channels                              ;
  total *= framesPerBuffer                                        ;
  switch (MMA(channel)->SampleFormat)                             {
    case paUInt8                                                : {
      uchar * data = (uchar *)MMA(channel)->Empty.data()          ;
      for (int i=0;i<total;i++) data[i] = 128                     ;
    }                                                             ;
    break                                                         ;
    case paInt16                                                : {
      short * data = (short *)MMA(channel)->Empty.data()          ;
      for (int i=0;i<total;i++) data[i] = 0                       ;
    }                                                             ;
    break                                                         ;
    case paInt32                                                : {
      int   * data = (int   *)MMA(channel)->Empty.data()          ;
      for (int i=0;i<total;i++) data[i] = 0                       ;
    }                                                             ;
    break                                                         ;
    case paFloat32                                              : {
      float * data = (float *)MMA(channel)->Empty.data()          ;
      for (int i=0;i<total;i++) data[i] = 0                       ;
    }                                                             ;
    break                                                         ;
  }                                                               ;
  memcpy(MMA(channel) -> Buffer . data ( )                        ,
         MMA(channel) -> Empty  . data ( )                        ,
         MMA(channel) -> BytesPerBuffer                         ) ;
  MMA(channel)->FramesPerBuffer = framesPerBuffer                 ;
#endif
}

bool N::VcfEditor::CreateChannel(int Channel)
{
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  if (AudioChannels.contains(Channel)) return false ;
  mmaAudio * mmaa = new mmaAudio()                  ;
  AudioChannels [ Channel ] = (void *)mmaa          ;
  mmaa -> Channel         = Channel                 ;
  mmaa -> Type            = 0                       ;
  mmaa -> BytesPerBuffer  = 0                       ;
  mmaa -> FramesPerBuffer = 0                       ;
  mmaa -> Plan            = Plan                    ;
  mmaa -> Multimedia      = this                    ;
  mmaa -> stream          = NULL                    ;
  mmaa -> Fed             = false                   ;
  mmaa -> Buffer . clear ( )                        ;
  mmaa -> Empty  . clear ( )                        ;
#endif
  return true                                       ;
}

void N::VcfEditor::ClearChannel(int Channel)
{ FunctionDebug ;
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  mmaAudio * mmaa = MMA(Channel) ;
  AudioChannels.remove (Channel) ;
  delete mmaa                    ;
#endif
}

int N::VcfEditor::FindChannel(void)
{
  int aido = 0                           ;
  do                                     {
    aido = rand()                        ;
  } while (AudioChannels.contains(aido)) ;
  return aido                            ;
}

bool N::VcfEditor::LoadAudioToFeeder(int channel,QString filename)
{ FunctionDebug ;
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  mmaFeeder * mmf = new mmaFeeder()                            ;
  mmf->Data = new nAudioData()                                 ;
  if (!Acoustics::Load(filename,*(mmf->Data)))       {
    delete mmf->Data                                           ;
    mmf->Data = NULL                                           ;
    delete mmf                                                 ;
    return false                                               ;
  }                                                            ;
  int rate     = mmf->Data->Rate()                             ;
  int bsize    = rate / 10                                     ;
  int fmt      = -1                                            ;
  switch (mmf->Data->Format())                                 {
    case Acoustics::UInt8  : fmt = paUInt8   ; break ;
    case Acoustics::Int16  : fmt = paInt16   ; break ;
    case Acoustics::Int32  : fmt = paInt32   ; break ;
    case Acoustics::Float32: fmt = paFloat32 ; break ;
    default                                                    :
      delete mmf->Data                                         ;
      mmf->Data = NULL                                         ;
      delete mmf                                               ;
    return false                                               ;
  }                                                            ;
  mmf->Channel      = channel                                  ;
  mmf->Type         = 0                                        ;
  mmf->SampleFormat = fmt                                      ;
  mmf->BufferFrames = bsize                                    ;
  mmf->Current      = 0                                        ;
  mmf->AudioSize    = mmf->Data->AudioSize()                   ;
  AudioFeeders [channel] = mmf                                 ;
#endif
  return true                                                  ;
}

bool N::VcfEditor::StartAudioByFeeder (int feeder,int channel)
{ FunctionDebug ;
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  int channels = MMF(feeder)->Data->Channels()             ;
  int rate     = MMF(feeder)->Data->Rate    ()             ;
  int fmt      = MMF(feeder)->SampleFormat                 ;
  int bsize    = MMF(feeder)->BufferFrames                 ;
  return OutputChannel(channel,-1,bsize,channels,fmt,rate) ;
#endif
  return false ;
}

bool N::VcfEditor::AttachFeeder(int feeder,int channel)
{ FunctionDebug ;
  OutputMaps[channel] = feeder ;
  return true                  ;
}

void N::VcfEditor::FeedChannel(int channel,int feeder,void * output)
{
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  if (!OutputMaps.contains(channel)) return                ;
  if (MMF(feeder)->Current>=MMF(feeder)->AudioSize)        {
    mmaFeeder * mmf = MMF(feeder)                          ;
    OutputMaps   . remove ( channel )                      ;
    AudioFeeders . remove ( feeder  )                      ;
    delete mmf->Data                                       ;
    mmf->Data = NULL                                       ;
    delete mmf                                             ;
    MMA(channel)->Answer = paComplete                      ;
    emit ShutdownAudio ( channel )                         ;
  } else                                                   {
    int    cs  = MMF(feeder )->Current                     ;
    int    ds  = MMF(feeder )->AudioSize                   ;
    int    bpf = MMA(channel)->BytesPerBuffer              ;
    char * da  = MMF(feeder )->Data->index(cs)             ;
    ds -= cs                                               ;
    if (ds>=bpf)                                           {
      MMF(feeder)->Current += bpf                          ;
      memcpy ( output , da , bpf )                         ;
    } else                                                 {
      uchar * ed = (uchar *)MMA(channel)->Empty  . data () ;
      uchar * ba = (uchar *)MMA(channel)->Buffer . data () ;
      memcpy ( ba     , ed , bpf )                         ;
      memcpy ( ba     , da , ds  )                         ;
      MMF(feeder)->Current = MMF(feeder)->AudioSize        ;
      memcpy ( output , ba , bpf )                         ;
    }                                                      ;
  }                                                        ;
#endif
}

bool N::VcfEditor::PlayAudioByFeeder(int channel,QString filename)
{ FunctionDebug ;
  if ( ! LoadAudioToFeeder ( channel , filename ) ) return false ;
  StartAudioByFeeder       ( channel , channel  )                ;
  return AttachFeeder      ( channel , channel  )                ;
}

bool N::VcfEditor::PlayAudioByFeeder(SUID uuid)
{ FunctionDebug ;
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  nAudioData * Data     = NULL                                   ;
  QString      filename = ""                                     ;
  bool         ready    = false                                  ;
  QString      suffix   = ""                                     ;
  EnterSQL(SC,plan->Sql)                                         ;
    QString   Q                                                  ;
    Q = SC.sql.SelectFrom                                        (
          "suffix"                                               ,
          TABLES(AudioFiles)                                     ,
          SC.WhereUuid(uuid))                                    ;
    if (SC.Fetch(Q)) suffix = SC.String(0)                       ;
    if (suffix.length()>0)                                       {
      if (suffix.toLower()=="raw")                               {
        QByteArray R                                             ;
        Q = SC.sql.SelectFrom                                    (
              "music"                                            ,
              TABLES(AudioFiles)                                 ,
              SC.WhereUuid(uuid)         )                       ;
        if (SC.Fetch(Q)) R = SC.ByteArray(0)                     ;
        if (R.size()>0)                                          {
          Data  = new nAudioData()                               ;
          Data -> resize(R.size())                               ;
          char * f = (char *)Data->data ()                       ;
          char * s = (char *)R    .data ()                       ;
          memcpy ( f , s , R . size () )                         ;
          R . clear ( )                                          ;
          ready = true                                           ;
        } else Data = NULL                                       ;
      } else                                                     {
        QDir dir                                                 ;
        QByteArray W                                             ;
        if (plan->Dirs.contains(Directory::Sounds))    {
          dir = plan->Dirs[Directory::Sounds]          ;
        } else                                                   {
          dir = plan->Temporary("")                              ;
        }                                                        ;
        filename = QString("%1.%2").arg(uuid).arg(suffix)        ;
        filename = dir . absoluteFilePath ( filename )           ;
        QFileInfo FI(filename)                                   ;
        if (!FI.exists())                                        {
          Q = SC.sql.SelectFrom                                  (
                "music"                                          ,
                TABLES(AudioFiles)                               ,
                SC.WhereUuid(uuid)                             ) ;
          if (SC.Fetch(Q)) W = SC.ByteArray(0)                   ;
          QFile F                                                ;
          F.setFileName(filename)                                ;
          if (F.open(QIODevice::WriteOnly))                      {
            F.write(W)                                           ;
            F.close()                                            ;
          }                                                      ;
        }                                                        ;
        QFileInfo FX(filename) ;
        ready = FX.exists()                                      ;
      }                                                          ;
    }                                                            ;
  LeaveSQL(SC,plan->Sql)                                         ;
  if (!ready) return false                                       ;
  int channel  = FindChannel()                                   ;
  if (NotNull(Data))                                             {
    mmaFeeder * mmf = new mmaFeeder()                            ;
    mmf->Data = Data                                             ;
    int rate     = mmf->Data->Rate()                             ;
    int bsize    = rate / 10                                     ;
    int fmt      = -1                                            ;
    switch (mmf->Data->Format())                                 {
      case Acoustics::UInt8  : fmt = paUInt8   ; break ;
      case Acoustics::Int16  : fmt = paInt16   ; break ;
      case Acoustics::Int32  : fmt = paInt32   ; break ;
      case Acoustics::Float32: fmt = paFloat32 ; break ;
      default                                                    :
        delete mmf->Data                                         ;
        mmf->Data = NULL                                         ;
        delete mmf                                               ;
      return false                                               ;
    }                                                            ;
    mmf->Channel      = channel                                  ;
    mmf->Type         = 0                                        ;
    mmf->SampleFormat = fmt                                      ;
    mmf->BufferFrames = bsize                                    ;
    mmf->Current      = 0                                        ;
    mmf->AudioSize    = mmf->Data->AudioSize()                   ;
    AudioFeeders [channel] = mmf                                 ;
    StartAudioByFeeder       ( channel , channel  )              ;
    return AttachFeeder      ( channel , channel  )              ;
  } else                                                         {
    if (filename.length()>0)                                     {
      CreateStream ( channel, uuid ,filename ) ;
//      if (!LoadAudioToFeeder ( channel , filename ) )            {
//        ready = false                                            ;
//      }                                                          ;
    } else ready = false                                         ;
  }                                                              ;
  if (!ready) return false                                       ;
//  StartAudioByFeeder       ( channel , channel  )                ;
//  return AttachFeeder      ( channel , channel  )                ;
#endif
  return true ;
}

bool N::VcfEditor::InputChannel(int Channel,int DeviceId,int BufferSize,int Channels,int SampleFormat,int SampleRate)
{ FunctionDebug ;
  return true ;
}

void N::VcfEditor::RecordAudio(int channel,const void * input,unsigned long framesPerBuffer)
{ FunctionDebug ;
}

bool N::VcfEditor::StartStream(int stream)
{ FunctionDebug ;
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  if (!StreamChannels.contains(stream)) return false ;

  #ifdef LINUX
  int th_retcode                        ;
  MMS(stream)->func = streamCallback    ;
  do                                    {
    th_retcode = pthread_create         (
                   &MMS(stream)->Thread ,
                   NULL                 ,
                   MMS(stream)->func    ,
                   (void *)MMS(stream)) ;
    if (th_retcode!=0) usleep(10*1000)  ;
  } while (th_retcode!=0)               ;
  MMS(stream)->Status = th_retcode      ;
  #endif

  #ifdef MACOSX
  int th_retcode                        ;
  MMS(stream)->func = streamCallback    ;
  do                                    {
    th_retcode = pthread_create         (
                   &MMS(stream)->Thread ,
                   NULL                 ,
                   MMS(stream)->func    ,
                   (void *)MMS(stream)) ;
    if (th_retcode!=0) usleep(10*1000)  ;
  } while (th_retcode!=0)               ;
  MMS(stream)->Status = th_retcode      ;
  #endif

  #ifdef WIN32
  MMS(stream)->func = streamCallback               ;
  do                                               {
    MMS(stream)->ReadThread                        =
      ::CreateThread                               (
        NULL                                       ,
        0                                          ,
        (LPTHREAD_START_ROUTINE)MMS(stream)->func  ,
        (LPVOID)MMS(stream)                        ,
        0                                          ,
        &MMS(stream)->dwThreadID                )  ;
    if (MMS(stream)->ReadThread==NULL) ::Sleep(10) ;
  } while (MMS(stream)->ReadThread==NULL)          ;
  MMS(stream)->Status = 0                          ;
  SetThreadPriority                                (
    MMS(stream)->ReadThread                        ,
    THREAD_PRIORITY_TIME_CRITICAL                ) ;
  #endif
#endif

  return true                                      ;
}

bool N::VcfEditor::CreateStream(int stream,SUID uuid,QString filename)
{ FunctionDebug ;
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  mmaStream * mmas = new mmaStream()                                ;
  ///////////////////////////////////////////////////////////////////
  mmas -> Plan            = Plan                                    ;
  mmas -> Multimedia      = this                                    ;
  mmas -> isAudio         = false                                   ;
  mmas -> isVideo         = false                                   ;
  mmas -> Type            = 2                                       ;
  mmas -> Stream          = stream                                  ;
  mmas -> Uuid            = uuid                                    ;
  mmas -> filename        = filename                                ;
  mmas -> pFormatCtx      = NULL                                    ;
  mmas -> vCodecCtx       = NULL                                    ;
  mmas -> aCodecCtx       = NULL                                    ;
  mmas -> vStream         = NULL                                    ;
  mmas -> aStream         = NULL                                    ;
  mmas -> vCodec          = NULL                                    ;
  mmas -> aCodec          = NULL                                    ;
  mmas -> pFrame          = NULL                                    ;
  mmas -> ConvertCtx      = NULL                                    ;
  mmas -> audioStream     = -1                                      ;
  mmas -> videoStream     = -1                                      ;
  mmas -> titleStream     = -1                                      ;
  mmas -> BytesPerSecond  = 0                                       ;
  mmas -> BytesPerSample  = 0                                       ;
  mmas -> SampleBits      = 0                                       ;
  mmas -> PlayBits        = 0                                       ;
  mmas -> Float           = 0                                       ;
  mmas -> AudioBufferSize = 0                                       ;
  mmas -> AudioLimit      = 0                                       ;
  mmas -> AudioSize       = 0                                       ;
  mmas -> AudioIndex      = 0                                       ;
  mmas -> AudioBytes      = 0                                       ;
  mmas -> ActualBytes     = 0                                       ;
  mmas -> DecodeLength    = 0                                       ;
  mmas -> DecodeBuffer    = NULL                                    ;
  mmas -> AudioBuffer     = NULL                                    ;
  mmas -> Streaming       = false                                   ;
  mmas -> ResampleCtx     = NULL                                    ;
  mmas -> ResampleBuffer  = NULL                                    ;
  mmas -> DstPerSample    = 0                                       ;
  mmas -> SampleSize      = 0                                       ;
  mmas -> SampleBucket    = 0                                       ;
  mmas -> VideoFresh      = QDateTime::currentDateTime()            ;
  mmas -> data[0]         = NULL                                    ;
  mmas -> data[1]         = NULL                                    ;
  mmas -> data[2]         = NULL                                    ;
  mmas -> data[3]         = NULL                                    ;
  ///////////////////////////////////////////////////////////////////
  QByteArray Native = File::toNative(filename)            ;
  if (::av_open_input_file(&mmas->pFormatCtx                        ,
             Native.constData(),NULL,0,NULL)!=0) return false       ;
  if (::av_find_stream_info(mmas->pFormatCtx)<0) return false       ;
  ///////////////////////////////////////////////////////////////////
  AVFormatContext * FormatCtx = mmas->pFormatCtx                    ;
  for (int i=0;i<(int)FormatCtx->nb_streams;i++)                    {
    if (FormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO &&
        mmas->videoStream < 0) mmas->videoStream = i                ;
    if (FormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_AUDIO &&
        mmas->audioStream < 0) mmas->audioStream = i                ;
  }                                                                 ;
  ///////////////////////////////////////////////////////////////////
  bool success = true                                               ;
  mmas->isVideo = (mmas->videoStream==-1) ? false : true            ;
  mmas->isAudio = (mmas->audioStream==-1) ? false : true            ;
  if (mmas->isVideo)                                                {
    mmas->vStream   = mmas->pFormatCtx->streams[mmas->videoStream]  ;
    mmas->vCodecCtx = mmas->vStream->codec                          ;
    mmas->vCodec    = ::avcodec_find_decoder                        (
                        mmas->vCodecCtx->codec_id                 ) ;
    if (IsNull(mmas->vCodec)) success = false                       ;
    if (success                                                    &&
        ::avcodec_open(mmas->vCodecCtx,mmas->vCodec)<0              )
        success = false                                             ;
  }                                                                 ;
  if (mmas->isAudio)                                                {
    mmas->aStream   = mmas->pFormatCtx->streams[mmas->audioStream]  ;
    mmas->aCodecCtx = mmas->aStream->codec                          ;
    mmas->aCodec    = ::avcodec_find_decoder                        (
                        mmas->aCodecCtx->codec_id                 ) ;
    if (IsNull(mmas->aCodec)) success = false                       ;
    if (success                                                    &&
        ::avcodec_open(mmas->aCodecCtx,mmas->aCodec)<0              )
        success = false                                             ;
  }                                                                 ;
  ///////////////////////////////////////////////////////////////////
  if (!success)                                                     {
    if (NotNull(mmas->pFormatCtx))                                  {
      ::av_close_input_file(mmas->pFormatCtx)                       ;
    }                                                               ;
    return false                                                    ;
  }                                                                 ;
  ///////////////////////////////////////////////////////////////////
  if (mmas->isAudio)                                                {
    int Bits = 0                                                    ;
    /////////////////////////////////////////////////////////////////
    mmas->BytesPerSecond  = mmas->aCodecCtx->sample_rate            ;
    mmas->BytesPerSecond *= mmas->aCodecCtx->channels               ;
    /////////////////////////////////////////////////////////////////
    int sampleFormat = mmas->aCodecCtx->sample_fmt                  ;
    int sampleRate   = mmas->aCodecCtx->sample_rate                 ;
    int channels     = mmas->aCodecCtx->channels                    ;
    /////////////////////////////////////////////////////////////////
    switch (sampleFormat)                                           {
      case AV_SAMPLE_FMT_U8                                         :
        Bits                =  8                                    ;
        mmas->PlayBits      =  8                                    ;
      break                                                         ;
      case AV_SAMPLE_FMT_S16                                        :
        Bits                  = 16                                  ;
        mmas->PlayBits        = 16                                  ;
        mmas->BytesPerSecond *=  2                                  ;
      break                                                         ;
      case AV_SAMPLE_FMT_S32                                        :
        Bits                  = 32                                  ;
        mmas->PlayBits        = 32                                  ;
        mmas->BytesPerSecond *=  4                                  ;
      break                                                         ;
      case AV_SAMPLE_FMT_FLT                                        :
        Bits                = 32                                    ;
        mmas->PlayBits        = 32                                  ;
        mmas->BytesPerSecond *=  4                                  ;
        mmas->Float           = true                                ;
      break                                                         ;
      case AV_SAMPLE_FMT_DBL                                        :
        Bits                  = sizeof(double) * 8                  ;
        mmas->PlayBits        = 32                                  ;
        mmas->BytesPerSecond *= sizeof(double)                      ;
        mmas ->ResampleCtx    = ::av_audio_resample_init            (
          channels   , channels                                     ,
          sampleRate , sampleRate                                   ,
          AV_SAMPLE_FMT_DBL                                         ,
          AV_SAMPLE_FMT_FLT                                         ,
          0, 0, 0, 0                                              ) ;
        mmas->SampleSize = ::av_get_bits_per_sample_format          (
                           AV_SAMPLE_FMT_DBL ) / 8                  ;
      break                                                         ;
      case AV_SAMPLE_FMT_NB                                         :
        Bits                =  0                                    ;
        mmas->PlayBits      =  0                                    ;
      break                                                         ;
      default                                                       :
        Bits                =  0                                    ;
        mmas->PlayBits      =  0                                    ;
      break                                                         ;
    }                                                               ;
    /////////////////////////////////////////////////////////////////
    mmas->SampleBits     = Bits                                     ;
    mmas->PlayBits      /= 8                                        ;
    mmas->BytesPerSample = mmas->BytesPerSecond / sampleRate        ;
    mmas->DecodeBuffer   = new unsigned char[AVCODEC_MAX_AUDIO_FRAME_SIZE*16] ;
    memset(mmas->DecodeBuffer,0,AVCODEC_MAX_AUDIO_FRAME_SIZE*16)              ;
    /////////////////////////////////////////////////////////////////////////////
    if (NotNull(mmas->ResampleCtx))                                             {
      mmas->ResampleBuffer = new unsigned char[AVCODEC_MAX_AUDIO_FRAME_SIZE*16] ;
      mmas->SampleBucket   = mmas->SampleSize * channels                        ;
      memset(mmas->ResampleBuffer,0,AVCODEC_MAX_AUDIO_FRAME_SIZE*16)            ;
    }                                                                           ;
  }                                                                             ;
  ///////////////////////////////////////////////////////////////////
  if (mmas->isVideo)                                                {
    mmas -> Dimension . setWidth  ( mmas->vCodecCtx->width  )       ;
    mmas -> Dimension . setHeight ( mmas->vCodecCtx->height )       ;
    mmas -> Duration  = mmas->pFormatCtx->duration                  ;
    mmas -> pFrame    = ::avcodec_alloc_frame()                     ;
    PlanDebug(10,tr("%1 x %2 : %3")
                .arg(mmas->Dimension.width ())
                .arg(mmas->Dimension.height())
                .arg(mmas->Duration)
                ) ;
  }                                                                 ;
  QTextCodec * codec = QTextCodec::codecForLocale()                 ;
  mmas ->Title = codec->toUnicode(mmas->pFormatCtx->title)          ;
  ///////////////////////////////////////////////////////////////////
  StreamChannels[stream] = mmas                                     ;
#endif
  return true                                                       ;
}

void N::VcfEditor::DestroyStream(int stream)
{ FunctionDebug ;
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  if (!StreamChannels.contains(stream)) return        ;
  /////////////////////////////////////////////////////
  mmaStream * mmas = MMS ( stream )                   ;
  StreamChannels . remove ( stream )                  ;
  /////////////////////////////////////////////////////
  if (mmas->ResampleCtx!=NULL                       ) {
    ::audio_resample_close(mmas->ResampleCtx)         ;
    delete [] mmas->ResampleBuffer                    ;
  }                                                   ;
  /////////////////////////////////////////////////////
  if (mmas->vCodecCtx     !=NULL && mmas->vCodec!=NULL) ::avcodec_close       (mmas->vCodecCtx ) ;
  if (mmas->aCodecCtx     !=NULL && mmas->aCodec!=NULL) ::avcodec_close       (mmas->aCodecCtx ) ;
  if (mmas->pFormatCtx    !=NULL                      ) ::av_close_input_file (mmas->pFormatCtx) ;
  if (mmas->pFrame        !=NULL                      ) ::av_free             (mmas->pFrame    ) ;
  if (mmas->DecodeBuffer  !=NULL                      ) delete [] mmas->DecodeBuffer             ;
  /////////////////////////////////////////////////////
  mmas->ResampleCtx    = NULL ;
  mmas->ResampleBuffer = NULL ;
  mmas->DecodeBuffer   = NULL ;
  mmas->vStream        = NULL ;
  mmas->aStream        = NULL ;
  mmas->vCodec         = NULL ;
  mmas->aCodec         = NULL ;
  mmas->vCodecCtx      = NULL ;
  mmas->aCodecCtx      = NULL ;
  mmas->pFormatCtx     = NULL ;
  mmas->pFrame         = NULL ;
#endif
}

void N::VcfEditor::RunStream(int stream)
{ FunctionDebug ;
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  if (!StreamChannels.contains(stream)) return    ;
  mmaStream * mmas = MMS ( stream )               ;
  /////////////////////////////////////////////////
  AVPacket * Packet = new AVPacket()              ;
  int rt                                          ;
  do                                              {
    rt = ::av_read_frame(mmas->pFormatCtx,Packet) ;
    if (rt==0)                                    {
    }                                             ;
  } while (rt==0)                                 ;
  ::av_free_packet(Packet)                        ;
#endif
}

bool N::VcfEditor::AttachDisplay(int stream,int display)
{
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  if (!StreamChannels .contains(stream )) return false ;
  if (!DisplayChannels.contains(display)) return false ;
  //////////////////////////////////////////////////////
  mmaStream  * mmas = MMS ( stream  )                  ;
  DisplaySDL * mmds = MMD ( display )                  ;
  //////////////////////////////////////////////////////
  unsigned char * data     [4]                         ;
  int             linesize [4]                         ;
  //////////////////////////////////////////////////////
  data    [0] = mmds->YUV->pixels  [0]                 ;
  data    [1] = mmds->YUV->pixels  [2]                 ;
  data    [2] = mmds->YUV->pixels  [1]                 ;
  data    [3] = NULL                                   ;
  linesize[0] = mmds->YUV->pitches [0]                 ;
  linesize[1] = mmds->YUV->pitches [2]                 ;
  linesize[2] = mmds->YUV->pitches [1]                 ;
  linesize[3] = 0                                      ;
  //////////////////////////////////////////////////////
  mmas->data    [0] = data    [0]                      ;
  mmas->data    [1] = data    [1]                      ;
  mmas->data    [2] = data    [2]                      ;
  mmas->data    [3] = data    [3]                      ;
  mmas->linesize[0] = linesize[0]                      ;
  mmas->linesize[1] = linesize[1]                      ;
  mmas->linesize[2] = linesize[2]                      ;
  mmas->linesize[3] = linesize[3]                      ;
  mmas->ConvertCtx  = ::sws_getContext                 (
                      mmas -> Dimension . width  ()    ,
                      mmas -> Dimension . height ()    ,
                      mmas->vCodecCtx->pix_fmt         ,
                      mmds->Rect . w                   ,
                      mmds->Rect . h                   ,
                      PIX_FMT_YUV420P                  ,
                      SWS_FAST_BILINEAR                ,
//      SWS_FAST_BILINEAR , SWS_BICUBIC , SWS_LANCZOS
                      NULL                             ,
                      NULL                             ,
                      NULL                           ) ;
#endif
  return true                                          ;
}

bool N::VcfEditor::LoadClipToStream(int stream,SUID uuid,QPoint corner)
{ FunctionDebug ;
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  QStringList Names                        ;
  QString filename = ""                    ;
  QSize   Dimension ( 0 , 0 )              ;
  nVideoManager VM(Plan)                   ;
  EnterSQL(SC,plan->Sql)                   ;
    Names     = VM.Names     ( SC , uuid ) ;
    filename  = VM.Filename  ( SC , uuid ) ;
    Dimension = VM.Dimension ( SC , uuid ) ;
  LeaveSQL(SC,plan->Sql)                   ;
  filename = plan->VideoFile(filename)     ;
#endif
  //////////////////////////////////////////
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  nMediaPlayer * MD = new nMediaPlayer(viewport(),Plan) ;
  QRect R(corner,Dimension) ;
  //////////////////////////////////////////////////////
  VcfInterface * vine = NULL                           ;
  vine = new VcfInterface ( this , NULL )              ;
  Scene . addItem ( vine )                             ;
  QRect RN( 0, 0,Dimension.width(),Dimension.height()) ;
  QRect RF( 0, 0,RN.width()+8,RN.height()+36)          ;
  QRect RC( 4,32,RN.width()  ,RN.height()   )          ;
  QRect RH( 4, 4,RN.width()  ,            24)          ;
  QPointF GC = mapToScene(corner)                      ;
  GC = Options.Standard(GC)                            ;
  vine -> Plan       = Plan                            ;
  vine -> Scaling    = true                            ;
  vine -> showHeader = true                            ;
  vine -> HeaderRect = RH                              ;
  vine -> setOptions ( Options , true     )            ;
  vine -> setZValue  ( 1.00f              )            ;
  vine -> setChild   ( MD  , GC , RF , RC )            ;
  Append ( vine , NULL )                               ;
  //////////////////////////////////////////////////////
  MD->show();
  MD->play(filename) ;
  //////////////////////////////////////////////////////
  if (Names.count()>0) vine -> Title = Names[0] ;
  vine->update() ;
  //////////////////////////////////////////////////////
//  return CreateStream ( stream, uuid ,filename ) ;
  CreateStream ( stream, uuid ,filename )        ;
  PlanDebug(10,tr("%1 x %2").arg(width()).arg(height()));
  QSize dim = MMS(stream)->Dimension             ;
  QRect view(0,0,dim.width(),dim.height())       ;
  PlanDebug(10,tr("Display->CreateView")) ;
  DisplaySDL * DSL = new DisplaySDL() ;
  DSL->Widget = new QGLWidget(QGLFormat(QGL::Rgba          |
                                        QGL::AlphaChannel  |
                                        QGL::DepthBuffer   |
                                        QGL::DoubleBuffer) ,viewport()) ;
  DSL->Widget->setGeometry(view) ;
  DSL->Widget->show() ;
  PlanDebug(10,tr("Create Widget")) ;
  plan->processEvents();
  int dbs = 24                             ;
  DSL->Widget->setAttribute ( Qt::WA_PaintOnScreen )            ;
  PlanDebug(10,tr("Qt::WA_PaintOnScreen")) ;
  ///////////////////////////////////////////////////////////////
  char variable[256]                                            ;
  ::sprintf ( variable , "SDL_WINDOWID=0x%lx" , DSL->Widget->winId() ) ;
  ::putenv  ( variable                                  )       ;
  PlanDebug(10,tr("SDL_InitSubSystem")) ;
  ///////////////////////////////////////////////////////////////
  ::SDL_InitSubSystem ( SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE ) ;
  ::SDL_Init          ( SDL_INIT_VIDEO                        ) ;
  ///////////////////////////////////////////////////////////////
  QSize sg = view.size()                                        ;
  PlanDebug(10,tr("SDL_SetVideoMode : %1 x %2").arg(sg.width()).arg(sg.height())) ;
  DSL->Screen = ::SDL_SetVideoMode                             (
                   sg . width  ()                               ,
                   sg . height ()                               ,
                   32                                           ,
                   SDL_OPENGL                                 ) ;
  PlanDebug(10,tr("SDL_SetVideoMode")) ;
  if (IsNull(DSL->Screen)) {
      PlanDebug("NO SDL") ;
  };
  ///////////////////////////////////////////////////////////////
  PlanDebug("SDL_GL_SetAttribute") ;
  ::SDL_GL_SetAttribute ( SDL_GL_RED_SIZE     , 8   )           ;
  ::SDL_GL_SetAttribute ( SDL_GL_GREEN_SIZE   , 8   )           ;
  ::SDL_GL_SetAttribute ( SDL_GL_BLUE_SIZE    , 8   )           ;
  ::SDL_GL_SetAttribute ( SDL_GL_ALPHA_SIZE   , 8   )           ;
  ::SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE   , dbs )           ;
  ::SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER , 1   )           ;
  ///////////////////////////////////////////////////////////////
  PlanDebug("SDL_CreateYUVOverlay") ;
  DSL->YUV    = ::SDL_CreateYUVOverlay    (
                view.width  ()                 ,
                view.height ()                 ,
                SDL_YV12_OVERLAY          ,
                DSL->Screen            ) ;
  /////////////////////////////////////////
  DSL->Rect.x  = view . left   ()         ;
  DSL->Rect.y  = view . top    ()         ;
  DSL->Rect.w  = view . width  ()         ;
  DSL->Rect.h  = view . height ()         ;
  /////////////////////////////////////////
  DisplayChannels[stream] = (void *)DSL          ;
  PlanDebug(10,tr("AttachDisplay")) ;
  AttachDisplay( stream , stream        )        ;
  StartStream  ( stream                 )        ;
#endif
  return true                                    ;
}

void N::VcfEditor::Queue(int Type,QPoint pos,const UUIDs & Uuids)
{
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  SUID uuid                               ;
  switch (Type)                           {
    case Types::Audio                     :
      foreach (uuid,Uuids)                {
        nVarArgs args                     ;
        args << "PlayAudioByFeeder"       ;
        args << uuid                      ;
        Commands << args                  ;
      }                                   ;
    break                                 ;
    case Types::Video                     :
      foreach (uuid,Uuids)                {
        int stream = FindChannel()        ;
        LoadClipToStream(stream,uuid,pos) ;
      }                                   ;
    break                                 ;
  }                                       ;
#endif
}

void N::VcfEditor::Prepare(void)
{
  Zoom    (                                            ) ;
  Default ( this,Scene,&Options                          ,
    SLOT(PaperMoving(QString,QPointF,QPointF,QPointF)) ) ;
  Zoom    (                                            ) ;
  FocusIn (                                            ) ;
  plan->processEvents ( )                                ;
  QMdiSubWindow * msw = Casting(QMdiSubWindow,parent() ) ;
  if (NotNull(msw)) Widgets[veMdi] = msw                 ;
  setWindowModified(false)                               ;
}

void N::VcfEditor::setModified(bool modified)
{
  bool oldModified = isWindowModified()          ;
  nDropOut ( nEqual ( oldModified , modified ) ) ;
  setWindowModified(modified)                    ;
  QString header                                 ;
  header  = tr("%1[%2]").arg(Title).arg(Version) ;
  if (isWindowModified()) header = "* " + header ;
  setWindowTitle(header)                         ;
  ////////////////////////////////////////////////
  QMdiSubWindow * msw = GetSubWindow()           ;
  if (NotNull(msw)) msw->setWindowTitle(header)  ;
}

void N::VcfEditor::setOriginate(SUID uuid,int sourceType)
{
  Group      = uuid       ;
  SourceType = sourceType ;
}

void N::VcfEditor::setTitle(QString title,QString version)
{
  QString header          ;
  Title   = title         ;
  Version = version       ;
  header  = tr("%1[%2]"   )
            .arg(Title    )
            .arg(Version) ;
  setWindowTitle(header)  ;
}

bool N::VcfEditor::loadVcf(SUID uuid,int mode)
{
  setEnabled ( false )                          ;
  VcfParagraph * paragraph = NULL               ;
  int            StartPage = 0                  ;
  QString        DL = "\n"                      ;
  SUID           FileUuid = 0                   ;
  Uuid = uuid                                   ;
  Mode = mode                                   ;
  GroupItems GI ( plan )                        ;
  EnterSQL ( SC , plan->sql )                   ;
    QString Q                                   ;
    FileUuid = GI.FindSecond                    (
                     SC,Uuid                    ,
                     Types::VCF       ,
                     Types::PlainText ,
                     Groups::Using    ,
            "order by position desc limit 0,1") ;
  LeaveSQL ( SC , plan->sql )                   ;
  TextDocument * NTD                            ;
  if (FileUuid<=0)                              {
    NTD = new TextDocument ( )                  ;
    NTD->body = DL.toUtf8()                     ;
  } else                                        {
    NTD = new TextDocument ( plan , FileUuid )  ;
  }                                             ;
  PlainUuids << Uuid                            ;
  Documents[Uuid] = NTD                         ;
  paragraph = addPlainText(StartPage,NTD)       ;
  setEnabled ( true  )                          ;
  Alert      ( Done  )                          ;
  return true                                   ;
}

bool N::VcfEditor::loadPlainText(SUID fileuuid)
{
  setEnabled ( false )                    ;
  VcfParagraph * paragraph = NULL         ;
  int            StartPage = 0            ;
  uuid = fileuuid                         ;
  Mode = PlainMode                        ;
  TextDocument * NTD                      ;
  NTD = new TextDocument ( plan , uuid )  ;
  PlainUuids << Uuid                      ;
  Documents[uuid] = NTD                   ;
  paragraph = addPlainText(StartPage,NTD) ;
  setEnabled ( true  )                    ;
  Alert      ( Done  )                    ;
  return true                             ;
}

void N::VcfEditor::ItemModified(VcfItem * item)
{
  setModified ( true ) ;
}

bool N::VcfEditor::Menu(QPoint pos)
{
  return false ;
}

QString N::VcfEditor::CursorPosition(QPointF paper)
{
  QString T                             ;
  QString X                             ;
  QString Y                             ;
  QString XD = ""                       ;
  QString YD = ""                       ;
  int     XV = (int)(paper.x() * 1000)  ;
  int     YV = (int)(paper.y() * 1000)  ;
  int     XW = paper.x()                ;
  int     YW = paper.y()                ;
  if (XV<0) XV = -XV                    ;
  if (YV<0) YV = -YV                    ;
  if (XW<0) XW = -XW                    ;
  if (YW<0) YW = -YW                    ;
  XW *= 1000                            ;
  YW *= 1000                            ;
  XV -= XW                              ;
  YV -= YW                              ;
  X   = QString::number((int)paper.x()) ;
  Y   = QString::number((int)paper.y()) ;
  X  += "."                             ;
  Y  += "."                             ;
  for (int i=0;i<3;i++)                 {
    XD  = QString::number(XV%10) + XD   ;
    YD  = QString::number(YV%10) + YD   ;
    XV /= 10                            ;
    YV /= 10                            ;
  }                                     ;
  X  += XD                              ;
  Y  += YD                              ;
  T   = tr("%1 x %2 cm").arg(X).arg(Y)  ;
  return T                              ;
}

void N::VcfEditor::PaperMoving(QString name,QPointF pos,QPointF paper,QPointF scene)
{
  QLabel * cursor = GetLabel ( veCursor ) ;
  QLabel * item   = GetLabel ( vePage   ) ;
  if (NotNull(cursor))                    {
    cursor->setText(CursorPosition(pos) ) ;
  }                                       ;
  if (NotNull(item  ))                    {
    QString m                             ;
    m = QString("%1/%2"                   )
        .arg(name                         )
        .arg(pages.count()              ) ;
    item  ->setText(m                   ) ;
  }                                       ;
}

void N::VcfEditor::Insert(void)
{
  AddPaper () ;
}

void N::VcfEditor::scrollUp(VcfParagraph * paragraph,QPointF pos)
{
  QScrollBar * sv = verticalScrollBar() ;
  nDropOut ( IsNull(sv) )               ;
  QPainterPath p                        ;
  int v = sv->value()                   ;
  do                                    {
    v--                                 ;
    if (v<0) v = 0                      ;
    sv->setValue(v)                     ;
    p = paragraph->visiblePath()        ;
    if (p.contains(pos))                {
      paragraph->update()               ;
      repaint  ()                       ;
      return                            ;
    }                                   ;
  } while (v>0)                         ;
}

void N::VcfEditor::scrollDown(VcfParagraph * paragraph,QPointF pos)
{
  QScrollBar * sv = verticalScrollBar()     ;
  nDropOut ( IsNull(sv) )                   ;
  QPainterPath p                            ;
  int v = sv->value()                       ;
  do                                        {
    v++                                     ;
    if (v>=sv->maximum()) v = sv->maximum() ;
    sv->setValue(v)                         ;
    p = paragraph->visiblePath()            ;
    if (p.contains(pos))                    {
      paragraph->update()                   ;
      repaint  ()                           ;
      return                                ;
    }                                       ;
  } while (v<sv->maximum())                 ;
}

void N::VcfEditor::acquirePaper(VcfParagraph * paragraph)
{
  AddPaper ( )                                          ;
  CUIDs   PUIDs                                         ;
  int     pid                                           ;
  QPointF SP(1.00,1.00)                                 ;
  QPointF TLP                                           ;
  QRectF  PX                                            ;
  QSizeF  PSF                                           ;
  PUIDs = pages.keys()                                  ;
  pid   = PUIDs.count() - 1                             ;
  PX    = pages[PUIDs[pid]]->PaperEditing()             ;
  TLP   = PX.topLeft()                                  ;
  PSF   = PX.size()                                     ;
  TLP  -= SP                                            ;
  PX    . setTopLeft(TLP)                               ;
  PX    . setSize   (PSF)                               ;
  pages[PUIDs[pid]]->Name = QString::number(PUIDs[pid]) ;
  pages[PUIDs[pid]]->Font = Options.fonts[0]            ;
  paragraph -> addPaper ( PX )                          ;
  paragraph -> Arrange  (    )                          ;
}

void N::VcfEditor::shrinkPaper(VcfParagraph * paragraph)
{
  CUIDs PUIDs = pages.keys()      ;
  nDropOut ( PUIDs.count() <= 0 ) ;
  int total = PUIDs.count()       ;
  nDropOut ( total <= 0         ) ;
  int pid   = PUIDs[total-1]      ;
  nDropOut (!pages.contains(pid)) ;
}

N::VcfParagraph * N::VcfEditor::addPlainText(int StartPage,TextDocument * document)
{
  VcfParagraph * paragraph                                        ;
  paragraph = new VcfParagraph ( this , NULL , plan )             ;
  scene ( ) -> addItem         ( paragraph        )               ;
  addItem                      ( paragraph , NULL )               ;
  connect(paragraph,SIGNAL(contentModified(VcfItem*))             ,
          this     ,SLOT  (ItemModified   (VcfItem*)) )           ;
  connect(paragraph,SIGNAL(Append(VcfItem*,VcfItem*))             ,
          this     ,SLOT  (Append(VcfItem*,VcfItem*)) )           ;
  connect(paragraph,SIGNAL(Remove(VcfItem*))                      ,
          this     ,SLOT  (Remove(VcfItem*)) )                    ;
  connect(paragraph,SIGNAL(scrollUp(VcfParagraph*,QPointF))       ,
          this     ,SLOT  (scrollUp(VcfParagraph*,QPointF)) )     ;
  connect(paragraph,SIGNAL(scrollDown(VcfParagraph*,QPointF))     ,
          this     ,SLOT  (scrollDown(VcfParagraph*,QPointF)) )   ;
  connect(paragraph,SIGNAL(ParagraphMenu(VcfParagraph*,QPointF))  ,
          this     ,SLOT  (ParagraphMenu(VcfParagraph*,QPointF))) ;
  connect(paragraph,SIGNAL(acquirePaper(VcfParagraph*))           ,
          this     ,SLOT  (acquirePaper(VcfParagraph*))         ) ;
  connect(paragraph,SIGNAL(shrinkPaper(VcfParagraph*))            ,
          this     ,SLOT  (shrinkPaper(VcfParagraph*))          ) ;
  /////////////////////////////////////////////////////////////////
  paragraph -> setOptions ( Options , true                  )     ;
  paragraph -> setZValue  ( 0.50f                           )     ;
  paragraph -> setOpacity ( 1.00f                           )     ;
  paragraph -> setRange   ( QRectF(1.00,1.00,19.00,27.70)   )     ;
  paragraph -> show       (                                 )     ;
  /////////////////////////////////////////////////////////////////
  paragraph -> Options -> LineSpace = 0.1                         ;
  paragraph -> Prepare    ( )                                     ;
  /////////////////////////////////////////////////////////////////
  CUIDs   PUIDs                                                   ;
  int     pid   = StartPage                                       ;
  QPointF SP(1.00,1.00)                                           ;
  QPointF TLP                                                     ;
  QRectF  PX                                                      ;
  QSizeF  PSF                                                     ;
  /////////////////////////////////////////////////////////////////
  Options.fonts[0]             = new VcfFont()                    ;
  Options.fonts[0]->font       = paragraph->Painter.fonts[8]      ;
  Options.fonts[0]->size       = paragraph->Painter.fonts[8].cm   ;
  Options.fonts[0]->pen        = QPen  (QColor(  0,  0,255))      ;
  Options.fonts[0]->penBrush   = QBrush(QColor(128,128,128))      ;
  Options.fonts[0]->brush      = QBrush(QColor(  0,  0,255))      ;
  Options.fonts[0]->selection  = QBrush(QColor(192,192,192))      ;
  paragraph->Options->fonts[0] = Options.fonts[0]                 ;
  /////////////////////////////////////////////////////////////////
  paragraph -> BeginPartition ( document )                        ;
  /////////////////////////////////////////////////////////////////
  do                                                              {
    PUIDs = pages.keys()                                          ;
    PX    = pages[PUIDs[pid]]->PaperEditing()                     ;
    TLP   = PX.topLeft()                                          ;
    PSF   = PX.size()                                             ;
    TLP  -= SP                                                    ;
    PX    . setTopLeft(TLP)                                       ;
    PX    . setSize   (PSF)                                       ;
    pages[PUIDs[pid]]->Name = QString::number(PUIDs[pid])         ;
    pages[PUIDs[pid]]->Font = Options.fonts[0]                    ;
    paragraph       -> addPaper  ( PX )                           ;
    pid = paragraph -> Partition (    )                           ;
    DoProcessEvents                                               ;
    if (pid>0)                                                    {
      if (pid>=PUIDs.count()) AddPaper()                          ;
    }                                                             ;
  } while (pid>0)                                                 ;
  /////////////////////////////////////////////////////////////////
  paragraph -> EndPartition (                                   ) ;
  /////////////////////////////////////////////////////////////////
  paragraph -> update     (                                     ) ;
  return paragraph                                                ;
}

QMdiSubWindow * N::VcfEditor::GetSubWindow(void)
{
  nKickOut ( !Widgets . contains (veMdi) , NULL )             ;
  QMdiSubWindow * msw = Casting(QMdiSubWindow,Widgets[veMdi]) ;
  nKickOut ( IsNull(msw) , NULL )                             ;
  return msw                                                  ;
}

QLabel * N::VcfEditor::GetLabel(int id)
{
  nKickOut ( !Widgets.contains(id) , NULL )    ;
  QLabel * label = Casting(QLabel,Widgets[id]) ;
  nKickOut ( IsNull(label) , NULL )            ;
  return label                                 ;
}

void N::VcfEditor::Print(void)
{
}

void N::VcfEditor::ExportPDF(void)
{
}

void N::VcfEditor::Export(void)
{
  nDropOut ( PlainUuids . count () <= 0 )         ;
  QString filename = QFileDialog::getSaveFileName (
                       this                       ,
                       tr("Export as ...")        ,
                       plan->Temporary("")        ,
                       "*.txt"                  ) ;
  nDropOut ( filename . length () <= 0 )          ;
  switch (Mode)                                   {
    case PlainMode                                :
      if (PlainUuids.contains(Uuid))              {
        TextDocument * doc = Documents[uuid]      ;
        if (IsNull(doc)) return                   ;
        doc -> Collect ( )                        ;
        QFile F(filename)                         ;
        if (F.open(QIODevice::WriteOnly))         {
          F . write ( doc->body )                 ;
          F . close (           )                 ;
          Alert     ( Done      )                 ;
        } else                                    {
          Alert     ( Error     )                 ;
        }                                         ;
      }                                           ;
    break                                         ;
  }                                               ;
}

void N::VcfEditor::Load(void)
{
}

void N::VcfEditor::Save(void)
{
  nDropOut ( !isWindowModified()        )                      ;
  nDropOut ( PlainUuids . count () <= 0 )                      ;
  switch (Mode)                                                {
    case PlainMode                                             :
      if (PlainUuids.contains(Uuid))                           {
        TextDocument * doc = Documents[uuid]                   ;
        nDropOut ( IsNull(doc) )                               ;
        doc -> Collect ( )                                     ;
        DocumentManager DM ( plan )                            ;
        EnterSQL( SC , plan->sql )                             ;
          DM.savePlainText(SC,uuid,doc->encoding,doc->body)    ;
          setModified  ( false                               ) ;
        LeaveSQL( SC , plan->sql )                             ;
        Alert   ( Done           )                             ;
      }                                                        ;
    break                                                      ;
    case DocumentMode                                          :
      if (Group>0 && Uuid>0)                                   {
        TextDocument * doc = Documents[uuid]                   ;
        nDropOut ( IsNull(doc) )                               ;
        doc -> Collect ( )                                     ;
        DocumentManager DM ( plan )                            ;
        EnterSQL( SC , plan->sql )                             ;
          QString Q                                            ;
          int encoding    = 1                                  ;
          SUID FileUuid   = 0                                  ;
          int  relation   = Groups :: Contains                 ;
          int  t1         = Types  :: VCF                      ;
          int  t2         = Types  :: PlainText                ;
          int  prefer     = 0                                  ;
          QDateTime Now   = QDateTime::currentDateTime()       ;
          QString   version                                    ;
          version = tr("%1:%2"                                 )
                    .arg(plan->Hostname                        )
                    .arg(Now.toString("yyyy/MM/dd-hh:mm:ss") ) ;
          Q = SC.sql.Update(TABLES(Groups)                     ,
                            QString                            (
                              "where first = %1 "
                              "and t1 = %2 "
                              "and t2 = %3 "
                              "and relation = %4"              )
                            .arg(Uuid                          )
                            .arg(t1                            )
                            .arg(t2                            )
                            .arg(Groups::Using       )
                            ,1,"relation"                    ) ;
          SC . Prepare ( Q                                   ) ;
          SC . Bind    ( "relation" , relation               ) ;
          SC . Exec    (                                     ) ;
          FileUuid = DM.acquirePlainText(SC)                   ;
          DM . savePlainText (SC,FileUuid,encoding,doc->body)  ;
          relation = Groups::Using                             ;
          Q = SC.sql.InsertInto                                (
                TABLES(Groups)                                 ,
                5                                              ,
                "first"                                        ,
                "second"                                       ,
                "t1"                                           ,
                "t2"                                           ,
                "relation"                                   ) ;
          SC . Prepare ( Q                                   ) ;
          SC . Bind    ( "first"    , Uuid                   ) ;
          SC . Bind    ( "second"   , FileUuid               ) ;
          SC . Bind    ( "t1"       , t1                     ) ;
          SC . Bind    ( "t2"       , t2                     ) ;
          SC . Bind    ( "relation" , relation               ) ;
          SC . Exec    (                                     ) ;
          Q = SC.sql.SelectFrom                                (
                "prefer"                                       ,
                 TABLES(DocumentGroups)                        ,
                 QString("where document = %1 "
                         "order by prefer desc "
                         "limit 0,1").arg(Group)             ) ;
          if (SC.Fetch(Q)) prefer = SC.Int(0)                  ;
          prefer++                                             ;
          Q = SC.sql.InsertInto                                (
                TABLES(DocumentGroups)                         ,
                5                                              ,
                "document"                                     ,
                "fileuuid"                                     ,
                "extension"                                    ,
                "prefer"                                       ,
                "version"                                    ) ;
          SC . Prepare ( Q                                   ) ;
          SC . Bind    ( "document"  , Group                 ) ;
          SC . Bind    ( "fileuuid"  , FileUuid              ) ;
          SC . Bind    ( "extension" , 737                   ) ;
          SC . Bind    ( "prefer"    , prefer                ) ;
          SC . Bind    ( "version"   , version.toUtf8()      ) ;
          SC . Exec    (                                     ) ;
          setModified  ( false                               ) ;
        LeaveSQL( SC , plan->Sql )                             ;
        Alert   ( Done           )                             ;
      }                                                        ;
    break                                                      ;
  }                                                            ;
}

void N::VcfEditor::SaveAs(void)
{
}

void N::VcfEditor::Close(void)
{
  QMdiSubWindow * msw = GetSubWindow ( ) ;
  Save        ( )                        ;
  deleteLater ( )                        ;
  if (NotNull(msw)) msw->deleteLater ( ) ;
}

void N::VcfEditor::Undo(void)
{
}

void N::VcfEditor::Redo(void)
{
}

void N::VcfEditor::Select(void)
{
}

void N::VcfEditor::Properties(void)
{
}

void N::VcfEditor::ParagraphMenu(VcfParagraph * paragraph,QPointF pos)
{
  nScopedMenu ( mm , this ) ;
  QAction * aa = NULL       ;
  mm.add(101,tr("Search"))  ;
  mm.setFont(plan)          ;
  aa = mm.exec()            ;
  if (IsNull(aa)) return    ;
  switch (mm[aa])           {
    case 101                :
    break                   ;
  }                         ;
}
