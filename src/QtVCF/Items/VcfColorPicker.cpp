#include <qtvcf.h>

N::VcfColorPicker:: VcfColorPicker ( VcfHeadParaments    )
                  : VcfCanvas      ( parent , item , p   )
                  , pen            ( NULL                )
                  , brush          ( NULL                )
                  , gradient       ( NULL                )
                  , bundle         ( NULL                )
                  , color          ( QColor(224,224,224) )
                  , selectMode     ( Graphics::HSV       )
{
  connect(this,SIGNAL(Menu      (VcfCanvas*,QPointF))   ,
          this,SLOT  (PickerMenu(VcfCanvas*,QPointF)) ) ;
}

N::VcfColorPicker::~VcfColorPicker (void)
{
}

void N::VcfColorPicker::Prepare(void)
{
  setPos  ( QPointF ( 1.0 , 1.0         ) ) ;
  setRect ( QRectF  ( 0 , 0 ,12.0 ,12.0 ) ) ;
  setHsv  (                               ) ;
}

void N::VcfColorPicker::setCorner(QPointF Pos)
{
  setPos ( Pos ) ;
}

void N::VcfColorPicker::Clear(void)
{
  for (int i=0;i<Related.count();i++) {
    emit RemoveItem ( Related[i] )    ;
  }                                   ;
  Related . clear ( )                 ;
}

void N::VcfColorPicker::setRgba(void)
{
  VcfColorSelector * vcs = new VcfColorSelector(GraphicsView(),this,plan) ;
  emit Append ( vcs , this )                                              ;
  vcs -> plan    = plan                                                   ;
  vcs -> Options = Options                                                ;
  vcs -> color   = &color                                                 ;
  vcs -> setSize    ( 256 ,  16          )                                ;
  vcs -> setPos     ( QPointF (  0,  0)  )                                ;
  vcs -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcs -> setZValue  ( 1.0                )                                ;
  vcs -> setOpacity ( 1.0                )                                ;
  vcs -> setPos     ( vcs->PaperMiddle() )                                ;
  vcs -> setR       ( Qt::Horizontal     )                                ;
  /////////////////////////////////////////////////////////////////////////
  VcfColorSelector * vcg = new VcfColorSelector(GraphicsView(),this,plan) ;
  emit Append ( vcg , this )                                              ;
  vcg -> plan    = plan                                                   ;
  vcg -> Options = Options                                                ;
  vcg -> color   = &color                                                 ;
  vcg -> setSize    ( 256, 16            )                                ;
  vcg -> setPos     ( QPointF (  0,  0)  )                                ;
  vcg -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcg -> setZValue  ( 1.0                )                                ;
  vcg -> setOpacity ( 1.0                )                                ;
  vcg -> setPos     ( vcg->PaperMiddle() )                                ;
  vcg -> setG       ( Qt::Horizontal     )                                ;
  /////////////////////////////////////////////////////////////////////////
  VcfColorSelector * vcb = new VcfColorSelector(GraphicsView(),this)      ;
  emit Append ( vcb , this )                                              ;
  vcb -> plan    = plan                                                   ;
  vcb -> Options = Options                                                ;
  vcb -> color   = &color                                                 ;
  vcb -> setSize    ( 256 , 16           )                                ;
  vcb -> setPos     ( QPointF (  0,  0)  )                                ;
  vcb -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcb -> setZValue  ( 1.0                )                                ;
  vcb -> setOpacity ( 1.0                )                                ;
  vcb -> setPos     ( vcb->PaperMiddle() )                                ;
  vcb -> setB       ( Qt::Horizontal     )                                ;
  /////////////////////////////////////////////////////////////////////////
  VcfColorSelector * vca = new VcfColorSelector(GraphicsView(),this)      ;
  emit Append ( vca , this )                                              ;
  vca -> plan    = plan                                                   ;
  vca -> Options = Options                                                ;
  vca -> color   = &color                                                 ;
  vca -> setSize    ( 256, 16            )                                ;
  vca -> setPos     ( QPointF (  0,  0)  )                                ;
  vca -> setCenter  ( QPointF (  0,  0)  )                                ;
  vca -> setZValue  ( 1.0                )                                ;
  vca -> setOpacity ( 1.0                )                                ;
  vca -> setPos     ( vca->PaperMiddle() )                                ;
  vca -> setA       ( Qt::Horizontal     )                                ;
  /////////////////////////////////////////////////////////////////////////
  Related << vcs                                                          ;
  Related << vcg                                                          ;
  Related << vcb                                                          ;
  Related << vca                                                          ;
  /////////////////////////////////////////////////////////////////////////
  connect(vcs ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcb ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcg ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vca ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcs ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  connect(vcb ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  connect(vcg ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  connect(vca ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> colorModel  = Graphics::RGBA                           ;
  vcs -> colorModel |= Graphics::BaseR                          ;
  vcb -> colorModel  = Graphics::RGBA                           ;
  vcb -> colorModel |= Graphics::BaseB                          ;
  vcg -> colorModel  = Graphics::RGBA                           ;
  vcg -> colorModel |= Graphics::BaseG                          ;
  vca -> colorModel  = Graphics::RGBA                           ;
  vca -> colorModel |= Graphics::BaseA                          ;
  selectMode         = Graphics::RGBA                           ;
  /////////////////////////////////////////////////////////////////////////
  double  hhhh = vcs->PaperRect.height()                                  ;
  QPointF vcpp = vcs->PaperMiddle() + QPointF(0.25,0.25)                  ;
  QPointF vcgp = vcg->PaperMiddle() + QPointF(0.25,0.50 + ( hhhh * 1 ) )  ;
  QPointF vcbp = vcb->PaperMiddle() + QPointF(0.25,0.75 + ( hhhh * 2 ) )  ;
  QPointF vcap = vca->PaperMiddle() + QPointF(0.25,1.00 + ( hhhh * 3 ) )  ;
  QRectF  pret (0,0,1,1)                                                  ;
  pret . setWidth  (   vcs->PaperRect.width ()       + 0.50 )             ;
  pret . setHeight ( ( vcs->PaperRect.height() * 4 ) + 1.25 )             ;
  vcs -> setPos ( vcpp )                                                  ;
  vcg -> setPos ( vcgp )                                                  ;
  vcb -> setPos ( vcbp )                                                  ;
  vca -> setPos ( vcap )                                                  ;
  vcs -> setFlag( ItemIsMovable , false )                                 ;
  vcg -> setFlag( ItemIsMovable , false )                                 ;
  vcb -> setFlag( ItemIsMovable , false )                                 ;
  vca -> setFlag( ItemIsMovable , false )                                 ;
  vcs -> setToolTip ( tr("Red channel"     ) )                            ;
  vcg -> setToolTip ( tr("Green channel"   ) )                            ;
  vcb -> setToolTip ( tr("Blue channel"    ) )                            ;
  vca -> setToolTip ( tr("Alpha channel"   ) )                            ;
         setToolTip ( tr("RGBA Color Model") )                            ;
  setRect       ( pret )                                                  ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> Scaling = false                                                  ;
  vcg -> Scaling = false                                                  ;
  vcb -> Scaling = false                                                  ;
  vca -> Scaling = false                                                  ;
         Scaling = false                                                  ;
  vcs -> update ( )                                                       ;
  vcg -> update ( )                                                       ;
  vcb -> update ( )                                                       ;
  vca -> update ( )                                                       ;
         update ( )                                                       ;
}

void N::VcfColorPicker::setCmyk(void)
{
  VcfColorSelector * vcs = new VcfColorSelector(GraphicsView(),this,plan) ;
  emit Append ( vcs , this )                                              ;
  vcs -> plan    = plan                                                   ;
  vcs -> Options = Options                                                ;
  vcs -> color   = &color                                                 ;
  vcs -> setSize    ( 256 , 16           )                                ;
  vcs -> setPos     ( QPointF (  0,  0)  )                                ;
  vcs -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcs -> setZValue  ( 1.0                )                                ;
  vcs -> setOpacity ( 1.0                )                                ;
  vcs -> setPos     ( vcs->PaperMiddle() )                                ;
  vcs -> setC       ( Qt::Horizontal     )                                ;
  /////////////////////////////////////////////////////////////////////////
  VcfColorSelector * vcg = new VcfColorSelector(GraphicsView(),this,plan) ;
  emit Append ( vcg , this )                                              ;
  vcg -> plan    = plan                                                   ;
  vcg -> Options = Options                                                ;
  vcg -> color   = &color                                                 ;
  vcg -> setSize    ( 256 , 16           )                                ;
  vcg -> setPos     ( QPointF (  0,  0)  )                                ;
  vcg -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcg -> setZValue  ( 1.0                )                                ;
  vcg -> setOpacity ( 1.0                )                                ;
  vcg -> setPos     ( vcg->PaperMiddle() )                                ;
  vcg -> setM       ( Qt::Horizontal     )                                ;
  /////////////////////////////////////////////////////////////////////////
  VcfColorSelector * vcb = new VcfColorSelector(GraphicsView(),this,plan) ;
  emit Append ( vcb , this )                                              ;
  vcb -> plan    = plan                                                   ;
  vcb -> Options = Options                                                ;
  vcb -> color   = &color                                                 ;
  vcb -> setSize    ( 256 , 16           )                                ;
  vcb -> setPos     ( QPointF (  0,  0)  )                                ;
  vcb -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcb -> setZValue  ( 1.0                )                                ;
  vcb -> setOpacity ( 1.0                )                                ;
  vcb -> setPos     ( vcb->PaperMiddle() )                                ;
  vcb -> setY       ( Qt::Horizontal     )                                ;
  /////////////////////////////////////////////////////////////////////////
  VcfColorSelector * vca = new VcfColorSelector(GraphicsView(),this,plan) ;
  emit Append ( vca , this )                                              ;
  vca -> plan    = plan                                                   ;
  vca -> Options = Options                                                ;
  vca -> color   = &color                                                 ;
  vca -> setSize    ( 256, 16            )                                ;
  vca -> setPos     ( QPointF (  0,  0)  )                                ;
  vca -> setCenter  ( QPointF (  0,  0)  )                                ;
  vca -> setZValue  ( 1.0                )                                ;
  vca -> setOpacity ( 1.0                )                                ;
  vca -> setPos     ( vca->PaperMiddle() )                                ;
  vca -> setK       ( Qt::Horizontal     )                                ;
  /////////////////////////////////////////////////////////////////////////
  Related << vcs                                                          ;
  Related << vcb                                                          ;
  Related << vcg                                                          ;
  Related << vca                                                          ;
  /////////////////////////////////////////////////////////////////////////
  connect(vcs ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcb ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcg ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vca ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcs ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  connect(vcb ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  connect(vcg ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  connect(vca ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> colorModel  = Graphics::CMYK                           ;
  vcs -> colorModel |= Graphics::BaseC                          ;
  vcb -> colorModel  = Graphics::CMYK                           ;
  vcb -> colorModel |= Graphics::BaseM                          ;
  vcg -> colorModel  = Graphics::CMYK                           ;
  vcg -> colorModel |= Graphics::BaseY                          ;
  vca -> colorModel  = Graphics::CMYK                           ;
  vca -> colorModel |= Graphics::BaseK                          ;
  selectMode         = Graphics::CMYK                           ;
  /////////////////////////////////////////////////////////////////////////
  double  hhhh = vcs->PaperRect.height()                                  ;
  QPointF vcpp = vcs->PaperMiddle() + QPointF(0.25,0.25)                  ;
  QPointF vcgp = vcg->PaperMiddle() + QPointF(0.25,0.50 + ( hhhh * 1 ) )  ;
  QPointF vcbp = vcb->PaperMiddle() + QPointF(0.25,0.75 + ( hhhh * 2 ) )  ;
  QPointF vcap = vca->PaperMiddle() + QPointF(0.25,1.00 + ( hhhh * 3 ) )  ;
  QRectF  pret (0,0,1,1)                                                  ;
  pret . setWidth  (   vcs->PaperRect.width ()       + 0.50 )             ;
  pret . setHeight ( ( vcs->PaperRect.height() * 4 ) + 1.25 )             ;
  vcs -> setPos ( vcpp )                                                  ;
  vcg -> setPos ( vcgp )                                                  ;
  vcb -> setPos ( vcbp )                                                  ;
  vca -> setPos ( vcap )                                                  ;
  vcs -> setFlag( ItemIsMovable , false )                                 ;
  vcg -> setFlag( ItemIsMovable , false )                                 ;
  vcb -> setFlag( ItemIsMovable , false )                                 ;
  vca -> setFlag( ItemIsMovable , false )                                 ;
  vcs -> setToolTip ( tr("Cyan channel"    ) )                            ;
  vcg -> setToolTip ( tr("Magenta channel" ) )                            ;
  vcb -> setToolTip ( tr("Yellow channel"  ) )                            ;
  vca -> setToolTip ( tr("Black channel"   ) )                            ;
         setToolTip ( tr("CMYK Color Model") )                            ;
  setRect       ( pret )                                                  ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> Scaling = false                                                  ;
  vcg -> Scaling = false                                                  ;
  vcb -> Scaling = false                                                  ;
  vca -> Scaling = false                                                  ;
         Scaling = false                                                  ;
  vcs -> update ( )                                                       ;
  vcg -> update ( )                                                       ;
  vcb -> update ( )                                                       ;
  vca -> update ( )                                                       ;
         update ( )                                                       ;
}

void N::VcfColorPicker::setHsv(void)
{
  VcfColorSelector * vcs = new VcfColorSelector(GraphicsView(),this,plan) ;
  emit Append ( vcs , this )                                              ;
  vcs -> plan    = plan                                                   ;
  vcs -> Options = Options                                                ;
  vcs -> color   = &color                                                 ;
  vcs -> setSize    ( 256 , 256          )                                ;
  vcs -> setPos     ( QPointF (  0,  0)  )                                ;
  vcs -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcs -> setZValue  ( 1.0                )                                ;
  vcs -> setOpacity ( 1.0                )                                ;
  vcs -> setPos     ( vcs->PaperMiddle() )                                ;
  vcs -> setHSV     ( color.valueF()     )                                ;
  /////////////////////////////////////////////////////////////////////////
  VcfColorSelector * vcb = new VcfColorSelector(GraphicsView(),this,plan) ;
  emit Append ( vcb , this )                                              ;
  vcb -> plan    = plan                                                   ;
  vcb -> Options = Options                                                ;
  vcb -> color   = &color                                                 ;
  vcb -> setSize    (  16 , 256          )                                ;
  vcb -> setPos     ( QPointF (  0,  0)  )                                ;
  vcb -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcb -> setZValue  ( 1.0                )                                ;
  vcb -> setOpacity ( 1.0                )                                ;
  vcb -> setPos     ( vcb->PaperMiddle() )                                ;
  vcb -> setHsvV    ( Qt::Vertical       )                                ;
  /////////////////////////////////////////////////////////////////////////
  Related << vcs                                                          ;
  Related << vcb                                                          ;
  /////////////////////////////////////////////////////////////////////////
  connect(vcs ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcb ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcs ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  connect(vcb ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> colorModel  = Graphics::HSV                            ;
  vcb -> colorModel  = Graphics::HSV                            ;
  vcb -> colorModel |= Graphics::BaseV                          ;
  selectMode         = Graphics::HSV                            ;
  /////////////////////////////////////////////////////////////////////////
  QPointF vcpp = vcs->PaperMiddle() + QPointF(0.25,0.25)                  ;
  QPointF vcbp = vcb->PaperMiddle() + QPointF(0.50,0.25)                  ;
  QRectF  pret (0,0,1,1)                                                  ;
  pret . setWidth  (vcs->PaperRect.width ()+vcb->PaperRect.width ()+0.75) ;
  pret . setHeight (vcs->PaperRect.height()+                        0.50) ;
  vcbp . setX ( vcbp.x() + vcs->PaperRect.width() )                       ;
  vcs -> setPos ( vcpp )                                                  ;
  vcb -> setPos ( vcbp )                                                  ;
  vcs -> setFlag( ItemIsMovable , false )                                 ;
  vcb -> setFlag( ItemIsMovable , false )                                 ;
  vcs -> setToolTip ( tr("H-S Plane") )                                   ;
  vcb -> setToolTip ( tr("V Value"  ) )                                   ;
         setToolTip ( tr("HSV Color Model") )                             ;
  setRect       ( pret )                                                  ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> Scaling = false                                                  ;
  vcb -> Scaling = false                                                  ;
         Scaling = false                                                  ;
  vcs -> update ( )                                                       ;
  vcb -> update ( )                                                       ;
         update ( )                                                       ;
}

void N::VcfColorPicker::setHsl(void)
{
  VcfColorSelector * vcs = new VcfColorSelector(GraphicsView(),this,plan) ;
  emit Append ( vcs , this )                                              ;
  vcs -> plan    = plan                                                   ;
  vcs -> Options = Options                                                ;
  vcs -> color   = &color                                                 ;
  vcs -> setSize    ( 256 , 256          )                                ;
  vcs -> setPos     ( QPointF (  0,  0)  )                                ;
  vcs -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcs -> setZValue  ( 1.0                )                                ;
  vcs -> setOpacity ( 1.0                )                                ;
  vcs -> setPos     ( vcs->PaperMiddle() )                                ;
  vcs -> setHSL     ( color.valueF()     )                                ;
  /////////////////////////////////////////////////////////////////////////
  VcfColorSelector * vcb = new VcfColorSelector(GraphicsView(),this,plan) ;
  emit Append ( vcb , this )                                              ;
  vcb -> plan    = plan                                                   ;
  vcb -> Options = Options                                                ;
  vcb -> color   = &color                                                 ;
  vcb -> setSize    (  16 , 256          )                                ;
  vcb -> setPos     ( QPointF (  0,  0)  )                                ;
  vcb -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcb -> setZValue  ( 1.0                )                                ;
  vcb -> setOpacity ( 1.0                )                                ;
  vcb -> setPos     ( vcb->PaperMiddle() )                                ;
  vcb -> setHslL    ( Qt::Vertical       )                                ;
  /////////////////////////////////////////////////////////////////////////
  Related << vcs                                                          ;
  Related << vcb                                                          ;
  /////////////////////////////////////////////////////////////////////////
  connect(vcs ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcb ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcs ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  connect(vcb ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> colorModel  = Graphics::HSL                            ;
  vcb -> colorModel  = Graphics::HSL                            ;
  vcb -> colorModel |= Graphics::BaseL                          ;
  selectMode         = Graphics::HSL                            ;
  /////////////////////////////////////////////////////////////////////////
  QPointF vcpp = vcs->PaperMiddle() + QPointF(0.25,0.25)                  ;
  QPointF vcbp = vcb->PaperMiddle() + QPointF(0.50,0.25)                  ;
  QRectF  pret (0,0,1,1)                                                  ;
  pret . setWidth  (vcs->PaperRect.width ()+vcb->PaperRect.width ()+0.75) ;
  pret . setHeight (vcs->PaperRect.height()+                        0.50) ;
  vcbp . setX ( vcbp.x() + vcs->PaperRect.width() )                       ;
  vcs -> setPos ( vcpp )                                                  ;
  vcb -> setPos ( vcbp )                                                  ;
  vcs -> setFlag( ItemIsMovable , false )                                 ;
  vcb -> setFlag( ItemIsMovable , false )                                 ;
  vcs -> setToolTip ( tr("H-S Plane") )                                   ;
  vcb -> setToolTip ( tr("L Value"  ) )                                   ;
         setToolTip ( tr("HSL Color Model") )                             ;
  setRect       ( pret )                                                  ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> Scaling = false                                                  ;
  vcb -> Scaling = false                                                  ;
         Scaling = false                                                  ;
  vcs -> update ( )                                                       ;
  vcb -> update ( )                                                       ;
         update ( )                                                       ;
}

void N::VcfColorPicker::setYuv(void)
{
  VcfColorSelector * vcs = new VcfColorSelector(GraphicsView(),this)      ;
  emit Append ( vcs , this )                                              ;
  ColorYUV YUV ( color )                                                  ;
  vcs -> plan    = plan                                                   ;
  vcs -> Options = Options                                                ;
  vcs -> color   = &color                                                 ;
  vcs -> setSize    ( 256 , 256          )                                ;
  vcs -> setPos     ( QPointF (  0,  0)  )                                ;
  vcs -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcs -> setZValue  ( 1.0                )                                ;
  vcs -> setOpacity ( 1.0                )                                ;
  vcs -> setPos     ( vcs->PaperMiddle() )                                ;
  vcs -> setYUV     ( YUV.Y              )                                ;
  /////////////////////////////////////////////////////////////////////////
  VcfColorSelector * vcb = new VcfColorSelector(GraphicsView(),this)      ;
  emit Append ( vcb , this )                                              ;
  vcb -> plan    = plan                                                   ;
  vcb -> Options = Options                                                ;
  vcb -> color   = &color                                                 ;
  vcb -> setSize    (  16 , 256          )                                ;
  vcb -> setPos     ( QPointF (  0,  0)  )                                ;
  vcb -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcb -> setZValue  ( 1.0                )                                ;
  vcb -> setOpacity ( 1.0                )                                ;
  vcb -> setPos     ( vcb->PaperMiddle() )                                ;
  vcb -> setYuvY    ( YUV.U , YUV.V , Qt::Vertical )                      ;
  /////////////////////////////////////////////////////////////////////////
  Related << vcs                                                          ;
  Related << vcb                                                          ;
  /////////////////////////////////////////////////////////////////////////
  connect(vcs ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcb ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcs ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  connect(vcb ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> colorModel  = Graphics::YUV                            ;
  vcb -> colorModel  = Graphics::YUV                            ;
  vcb -> colorModel |= Graphics::BaseY                          ;
  selectMode         = Graphics::YUV                            ;
  /////////////////////////////////////////////////////////////////////////
  QPointF vcpp = vcs->PaperMiddle() + QPointF(0.25,0.25)                  ;
  QPointF vcbp = vcb->PaperMiddle() + QPointF(0.50,0.25)                  ;
  QRectF  pret (0,0,1,1)                                                  ;
  pret . setWidth  (vcs->PaperRect.width ()+vcb->PaperRect.width ()+0.75) ;
  pret . setHeight (vcs->PaperRect.height()+                        0.50) ;
  vcbp . setX ( vcbp.x() + vcs->PaperRect.width() )                       ;
  vcs -> setPos ( vcpp )                                                  ;
  vcb -> setPos ( vcbp )                                                  ;
  vcs -> setFlag( ItemIsMovable , false )                                 ;
  vcb -> setFlag( ItemIsMovable , false )                                 ;
  vcs -> setToolTip ( tr("U-V Plane") )                                   ;
  vcb -> setToolTip ( tr("Y Value"  ) )                                   ;
         setToolTip ( tr("YUV Color Model") )                             ;
  setRect       ( pret )                                                  ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> Scaling = false                                                  ;
  vcb -> Scaling = false                                                  ;
         Scaling = false                                                  ;
  vcs -> update ( )                                                       ;
  vcb -> update ( )                                                       ;
         update ( )                                                       ;
}

void N::VcfColorPicker::setYiq(void)
{
  VcfColorSelector * vcs = new VcfColorSelector(GraphicsView(),this)      ;
  emit Append ( vcs , this )                                              ;
  ColorYIQ YIQ ( color )                                                  ;
  vcs -> plan    = plan                                                   ;
  vcs -> Options = Options                                                ;
  vcs -> color   = &color                                                 ;
  vcs -> setSize    ( 256 , 256          )                                ;
  vcs -> setPos     ( QPointF (  0,  0)  )                                ;
  vcs -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcs -> setZValue  ( 1.0                )                                ;
  vcs -> setOpacity ( 1.0                )                                ;
  vcs -> setPos     ( vcs->PaperMiddle() )                                ;
  vcs -> setYIQ     ( YIQ.Y              )                                ;
  /////////////////////////////////////////////////////////////////////////
  VcfColorSelector * vcb = new VcfColorSelector(GraphicsView(),this)      ;
  emit Append ( vcb , this )                                              ;
  vcb -> plan    = plan                                                   ;
  vcb -> Options = Options                                                ;
  vcb -> color   = &color                                                 ;
  vcb -> setSize    (  16 , 256          )                                ;
  vcb -> setPos     ( QPointF (  0,  0)  )                                ;
  vcb -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcb -> setZValue  ( 1.0                )                                ;
  vcb -> setOpacity ( 1.0                )                                ;
  vcb -> setPos     ( vcb->PaperMiddle() )                                ;
  vcb -> setYiqY    ( YIQ.I , YIQ.Q , Qt::Vertical )                      ;
  /////////////////////////////////////////////////////////////////////////
  Related << vcs                                                          ;
  Related << vcb                                                          ;
  /////////////////////////////////////////////////////////////////////////
  connect(vcs ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcb ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcs ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  connect(vcb ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> colorModel  = Graphics::YIQ                            ;
  vcb -> colorModel  = Graphics::YIQ                            ;
  vcb -> colorModel |= Graphics::BaseY                          ;
  selectMode         = Graphics::YIQ                            ;
  /////////////////////////////////////////////////////////////////////////
  QPointF vcpp = vcs->PaperMiddle() + QPointF(0.25,0.25)                  ;
  QPointF vcbp = vcb->PaperMiddle() + QPointF(0.50,0.25)                  ;
  QRectF  pret (0,0,1,1)                                                  ;
  pret . setWidth  (vcs->PaperRect.width ()+vcb->PaperRect.width ()+0.75) ;
  pret . setHeight (vcs->PaperRect.height()+                        0.50) ;
  vcbp . setX ( vcbp.x() + vcs->PaperRect.width() )                       ;
  vcs -> setPos ( vcpp )                                                  ;
  vcb -> setPos ( vcbp )                                                  ;
  vcs -> setFlag( ItemIsMovable , false )                                 ;
  vcb -> setFlag( ItemIsMovable , false )                                 ;
  vcs -> setToolTip ( tr("I-Q Plane") )                                   ;
  vcb -> setToolTip ( tr("Y Value"  ) )                                   ;
         setToolTip ( tr("YIQ Color Model") )                             ;
  setRect       ( pret )                                                  ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> Scaling = false                                                  ;
  vcb -> Scaling = false                                                  ;
         Scaling = false                                                  ;
  vcs -> update ( )                                                       ;
  vcb -> update ( )                                                       ;
         update ( )                                                       ;
}

void N::VcfColorPicker::setXyz(void)
{
  VcfColorSelector * vcs = new VcfColorSelector(GraphicsView(),this)      ;
  emit Append ( vcs , this )                                              ;
  ColorXYZ XYZ ( color )                                                  ;
  vcs -> plan    = plan                                                   ;
  vcs -> Options = Options                                                ;
  vcs -> color   = &color                                                 ;
  vcs -> setSize    ( 256 , 256          )                                ;
  vcs -> setPos     ( QPointF (  0,  0)  )                                ;
  vcs -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcs -> setZValue  ( 1.0                )                                ;
  vcs -> setOpacity ( 1.0                )                                ;
  vcs -> setPos     ( vcs->PaperMiddle() )                                ;
  vcs -> setXYZ     ( XYZ.Y              )                                ;
  /////////////////////////////////////////////////////////////////////////
  VcfColorSelector * vcb = new VcfColorSelector(GraphicsView(),this,plan) ;
  emit Append ( vcb , this )                                              ;
  vcb -> plan    = plan                                                   ;
  vcb -> Options = Options                                                ;
  vcb -> color   = &color                                                 ;
  vcb -> setSize    (  16 , 256          )                                ;
  vcb -> setPos     ( QPointF (  0,  0)  )                                ;
  vcb -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcb -> setZValue  ( 1.0                )                                ;
  vcb -> setOpacity ( 1.0                )                                ;
  vcb -> setPos     ( vcb->PaperMiddle() )                                ;
  vcb -> setXyzY    ( XYZ.X , XYZ.Z , Qt::Vertical )                      ;
  /////////////////////////////////////////////////////////////////////////
  Related << vcs                                                          ;
  Related << vcb                                                          ;
  /////////////////////////////////////////////////////////////////////////
  connect(vcs ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcb ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcs ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  connect(vcb ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> colorModel  = Graphics::XYZ                            ;
  vcb -> colorModel  = Graphics::XYZ                            ;
  vcb -> colorModel |= Graphics::BaseY                          ;
  selectMode         = Graphics::XYZ                            ;
  /////////////////////////////////////////////////////////////////////////
  QPointF vcpp = vcs->PaperMiddle() + QPointF(0.25,0.25)                  ;
  QPointF vcbp = vcb->PaperMiddle() + QPointF(0.50,0.25)                  ;
  QRectF  pret (0,0,1,1)                                                  ;
  pret . setWidth  (vcs->PaperRect.width ()+vcb->PaperRect.width ()+0.75) ;
  pret . setHeight (vcs->PaperRect.height()+                        0.50) ;
  vcbp . setX ( vcbp.x() + vcs->PaperRect.width() )                       ;
  vcs -> setPos ( vcpp )                                                  ;
  vcb -> setPos ( vcbp )                                                  ;
  vcs -> setFlag( ItemIsMovable , false )                                 ;
  vcb -> setFlag( ItemIsMovable , false )                                 ;
  vcs -> setToolTip ( tr("X-Z Plane") )                                   ;
  vcb -> setToolTip ( tr("Y Value"  ) )                                   ;
         setToolTip ( tr("XYZ Color Model") )                             ;
  setRect       ( pret )                                                  ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> Scaling = false                                                  ;
  vcb -> Scaling = false                                                  ;
         Scaling = false                                                  ;
  vcs -> update ( )                                                       ;
  vcb -> update ( )                                                       ;
         update ( )                                                       ;
}

void N::VcfColorPicker::setLab(void)
{
  VcfColorSelector * vcs = new VcfColorSelector(GraphicsView(),this)      ;
  emit Append ( vcs , this )                                              ;
  ColorLab Lab ( color )                                                  ;
  vcs -> plan    = plan                                                   ;
  vcs -> Options = Options                                                ;
  vcs -> color   = &color                                                 ;
  vcs -> setSize    ( 256,256            )                                ;
  vcs -> setPos     ( QPointF (  0,  0)  )                                ;
  vcs -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcs -> setZValue  ( 1.0                )                                ;
  vcs -> setOpacity ( 1.0                )                                ;
  vcs -> setPos     ( vcs->PaperMiddle() )                                ;
  vcs -> setLab     ( Lab.L              )                                ;
  /////////////////////////////////////////////////////////////////////////
  VcfColorSelector * vcb = new VcfColorSelector(GraphicsView(),this)      ;
  emit Append ( vcb , this )                                              ;
  vcb -> plan    = plan                                                   ;
  vcb -> Options = Options                                                ;
  vcb -> color   = &color                                                 ;
  vcb -> setSize    ( 16 , 256           )                                ;
  vcb -> setPos     ( QPointF (  0,  0)  )                                ;
  vcb -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcb -> setZValue  ( 1.0                )                                ;
  vcb -> setOpacity ( 1.0                )                                ;
  vcb -> setPos     ( vcb->PaperMiddle() )                                ;
  vcb -> setLabL    ( Lab.a , Lab.b , Qt::Vertical )                      ;
  /////////////////////////////////////////////////////////////////////////
  Related << vcs                                                          ;
  Related << vcb                                                          ;
  /////////////////////////////////////////////////////////////////////////
  connect(vcs ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcb ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcs ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  connect(vcb ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> colorModel  = Graphics::Lab                            ;
  vcb -> colorModel  = Graphics::Lab                            ;
  vcb -> colorModel |= Graphics::BaseL                          ;
  selectMode         = Graphics::Lab                            ;
  /////////////////////////////////////////////////////////////////////////
  QPointF vcpp = vcs->PaperMiddle() + QPointF(0.25,0.25)                  ;
  QPointF vcbp = vcb->PaperMiddle() + QPointF(0.50,0.25)                  ;
  QRectF  pret (0,0,1,1)                                                  ;
  pret . setWidth  (vcs->PaperRect.width ()+vcb->PaperRect.width ()+0.75) ;
  pret . setHeight (vcs->PaperRect.height()+                        0.50) ;
  vcbp . setX ( vcbp.x() + vcs->PaperRect.width() )                       ;
  vcs -> setPos ( vcpp )                                                  ;
  vcb -> setPos ( vcbp )                                                  ;
  vcs -> setFlag( ItemIsMovable , false )                                 ;
  vcb -> setFlag( ItemIsMovable , false )                                 ;
  vcs -> setToolTip ( tr("a-b Plane") )                                   ;
  vcb -> setToolTip ( tr("L Value"  ) )                                   ;
         setToolTip ( tr("Lab Color Model") )                             ;
  setRect       ( pret )                                                  ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> Scaling = false                                                  ;
  vcb -> Scaling = false                                                  ;
         Scaling = false                                                  ;
  vcs -> update ( )                                                       ;
  vcb -> update ( )                                                       ;
         update ( )                                                       ;
}

void N::VcfColorPicker::setLuv(void)
{
  VcfColorSelector * vcs = new VcfColorSelector(GraphicsView(),this)      ;
  emit Append ( vcs , this )                                              ;
  ColorLUV LUV ( color )                                                  ;
  vcs -> plan    = plan                                                   ;
  vcs -> Options = Options                                                ;
  vcs -> color   = &color                                                 ;
  vcs -> setSize    ( 256 , 256          )                                ;
  vcs -> setPos     ( QPointF (  0,  0)  )                                ;
  vcs -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcs -> setZValue  ( 1.0                )                                ;
  vcs -> setOpacity ( 1.0                )                                ;
  vcs -> setPos     ( vcs->PaperMiddle() )                                ;
  vcs -> setLuv     ( LUV.L              )                                ;
  /////////////////////////////////////////////////////////////////////////
  VcfColorSelector * vcb = new VcfColorSelector(GraphicsView(),this)      ;
  emit Append ( vcb , this )                                              ;
  vcb -> plan    = plan                                                   ;
  vcb -> Options = Options                                                ;
  vcb -> color   = &color                                                 ;
  vcb -> setSize    (  16 , 256          )                                ;
  vcb -> setPos     ( QPointF (  0,  0)  )                                ;
  vcb -> setCenter  ( QPointF (  0,  0)  )                                ;
  vcb -> setZValue  ( 1.0                )                                ;
  vcb -> setOpacity ( 1.0                )                                ;
  vcb -> setPos     ( vcb->PaperMiddle() )                                ;
  vcb -> setLuvL    ( LUV.U , LUV.V , Qt::Vertical )                      ;
  /////////////////////////////////////////////////////////////////////////
  Related << vcs                                                          ;
  Related << vcb                                                          ;
  /////////////////////////////////////////////////////////////////////////
  connect(vcs ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcb ,SIGNAL(Update      (VcfColorSelector*))                    ,
          this,SLOT  (PickerUpdate(VcfColorSelector*))   )                ;
  connect(vcs ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  connect(vcb ,SIGNAL(Menu        (VcfPicture*,QPointF))                  ,
          this,SLOT  (PickerMenu  (VcfPicture*,QPointF)) )                ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> colorModel  = Graphics::LUV                            ;
  vcb -> colorModel  = Graphics::LUV                            ;
  vcb -> colorModel |= Graphics::BaseL                          ;
  selectMode         = Graphics::LUV                            ;
  /////////////////////////////////////////////////////////////////////////
  QPointF vcpp = vcs->PaperMiddle() + QPointF(0.25,0.25)                  ;
  QPointF vcbp = vcb->PaperMiddle() + QPointF(0.50,0.25)                  ;
  QRectF  pret (0,0,1,1)                                                  ;
  pret . setWidth  (vcs->PaperRect.width ()+vcb->PaperRect.width ()+0.75) ;
  pret . setHeight (vcs->PaperRect.height()+                        0.50) ;
  vcbp . setX ( vcbp.x() + vcs->PaperRect.width() )                       ;
  vcs -> setPos ( vcpp )                                                  ;
  vcb -> setPos ( vcbp )                                                  ;
  vcs -> setFlag( ItemIsMovable , false )                                 ;
  vcb -> setFlag( ItemIsMovable , false )                                 ;
  vcs -> setToolTip ( tr("U-V Plane") )                                   ;
  vcb -> setToolTip ( tr("L Value"  ) )                                   ;
         setToolTip ( tr("LUV Color Model") )                             ;
  setRect       ( pret )                                                  ;
  /////////////////////////////////////////////////////////////////////////
  vcs -> Scaling = false                                                  ;
  vcb -> Scaling = false                                                  ;
         Scaling = false                                                  ;
  vcs -> update ( )                                                       ;
  vcb -> update ( )                                                       ;
         update ( )                                                       ;
}

void N::VcfColorPicker::PickerMenu(VcfCanvas * canvas,QPointF pos)
{
  PickerMenu ( canvas->toGlobal(pos) ) ;
}

void N::VcfColorPicker::PickerMenu(VcfPicture * picture,QPointF pos)
{
  PickerMenu ( picture->toGlobal(pos) ) ;
}

bool N::VcfColorPicker::PickerMenu(QPoint pos)
{
  VcfScopedMenu ( mm )                           ;
  QAction    * a                                 ;
      mm.add  ( 101 , tr("Finish") )             ;
      mm.addSeparator (            )             ;
      mm.add  ( 201 , tr("RGBA"  ) )             ;
      mm.add  ( 202 , tr("CYMK"  ) )             ;
      mm.add  ( 203 , tr("HSV"   ) )             ;
      mm.add  ( 204 , tr("HSL"   ) )             ;
      mm.add  ( 205 , tr("YUV"   ) )             ;
      mm.add  ( 206 , tr("YIQ"   ) )             ;
      mm.add  ( 207 , tr("XYZ"   ) )             ;
      mm.add  ( 208 , tr("Lab"   ) )             ;
      mm.add  ( 209 , tr("LUV"   ) )             ;
  mm . setFont( plan               )             ;
  a = mm.exec ( pos                )             ;
  if (IsNull(a)) return false                    ;
  switch (mm[a])                                 {
    case 101                                     :
      Clear ( )                                  ;
      emit RemoveItem ( this )                   ;
    break                                        ;
    case 201                                     :
      Clear   ( )                                ;
      setRgba ( )                                ;
    break                                        ;
    case 202                                     :
      Clear   ( )                                ;
      setCmyk ( )                                ;
    break                                        ;
    case 203                                     :
      Clear   ( )                                ;
      setHsv  ( )                                ;
    break                                        ;
    case 204                                     :
      Clear   ( )                                ;
      setHsl  ( )                                ;
    break                                        ;
    case 205                                     :
      Clear   ( )                                ;
      setYuv  ( )                                ;
    break                                        ;
    case 206                                     :
      Clear   ( )                                ;
      setYiq  ( )                                ;
    break                                        ;
    case 207                                     :
      Clear   ( )                                ;
      setXyz  ( )                                ;
    break                                        ;
    case 208                                     :
      Clear   ( )                                ;
      setLab  ( )                                ;
    break                                        ;
    case 209                                     :
      Clear   ( )                                ;
      setLuv  ( )                                ;
    break                                        ;
  }                                              ;
  return true                                    ;
}

void N::VcfColorPicker::PickerUpdate(VcfColorSelector *)
{
  VcfColorSelector * vcs = NULL                       ;
  VcfColorSelector * vcb = NULL                       ;
  VcfColorSelector * vcg = NULL                       ;
  VcfColorSelector * vca = NULL                       ;
  plan -> processEvents ( )                           ;
  switch (selectMode)                                 {
    case Graphics::RGBA :                             {
      vcs  = (VcfColorSelector *)Related[0]           ;
      vcg  = (VcfColorSelector *)Related[1]           ;
      vcb  = (VcfColorSelector *)Related[2]           ;
      vca  = (VcfColorSelector *)Related[3]           ;
      vcs -> setR ( Qt::Horizontal )                  ;
      vcg -> setG ( Qt::Horizontal )                  ;
      vcb -> setB ( Qt::Horizontal )                  ;
      vca -> setA ( Qt::Horizontal )                  ;
    }                                                 ;
    break                                             ;
    case Graphics::CMYK :                             {
      vcs  = (VcfColorSelector *)Related[0]           ;
      vcg  = (VcfColorSelector *)Related[1]           ;
      vcb  = (VcfColorSelector *)Related[2]           ;
      vca  = (VcfColorSelector *)Related[3]           ;
      vcs -> setC ( Qt::Horizontal )                  ;
      vcg -> setM ( Qt::Horizontal )                  ;
      vcb -> setY ( Qt::Horizontal )                  ;
      vca -> setK ( Qt::Horizontal )                  ;
    }                                                 ;
    break                                             ;
    case Graphics::HSV                                :
      vcs  = (VcfColorSelector *)Related[0]           ;
      vcb  = (VcfColorSelector *)Related[1]           ;
      vcs -> setHSV  ( color.valueF() )               ;
      vcb -> setHsvV ( Qt::Vertical   )               ;
    break                                             ;
    case Graphics::HSL                                :
      vcs  = (VcfColorSelector *)Related[0]           ;
      vcb  = (VcfColorSelector *)Related[1]           ;
      vcs -> setHSL  ( color.valueF() )               ;
      vcb -> setHslL ( Qt::Vertical   )               ;
    break                                             ;
    case Graphics::YUV :                              {
      ColorYUV YUV ( color )                          ;
      vcs  = (VcfColorSelector *)Related[0]           ;
      vcb  = (VcfColorSelector *)Related[1]           ;
      vcs -> setYUV  ( YUV.Y                        ) ;
      vcb -> setYuvY ( YUV.U , YUV.V , Qt::Vertical ) ;
    }                                                 ;
    break                                             ;
    case Graphics::YIQ :                              {
      ColorYIQ YIQ ( color )                          ;
      vcs  = (VcfColorSelector *)Related[0]           ;
      vcb  = (VcfColorSelector *)Related[1]           ;
      vcs -> setYIQ  ( YIQ.Y                        ) ;
      vcb -> setYiqY ( YIQ.I , YIQ.Q , Qt::Vertical ) ;
    }                                                 ;
    break                                             ;
    case Graphics::XYZ :                              {
      ColorXYZ XYZ ( color )                          ;
      vcs  = (VcfColorSelector *)Related[0]           ;
      vcb  = (VcfColorSelector *)Related[1]           ;
      vcs -> setXYZ  ( XYZ.Y                        ) ;
      vcb -> setXyzY ( XYZ.X , XYZ.Z , Qt::Vertical ) ;
    }                                                 ;
    break                                             ;
    case Graphics::Lab :                              {
      ColorLab Lab ( color )                          ;
      vcs  = (VcfColorSelector *)Related[0]           ;
      vcb  = (VcfColorSelector *)Related[1]           ;
      vcs -> setLab  ( Lab.L                        ) ;
      vcb -> setLabL ( Lab.a , Lab.b , Qt::Vertical ) ;
    }                                                 ;
    break                                             ;
    case Graphics::LUV :                              {
      ColorLUV LUV ( color )                          ;
      vcs  = (VcfColorSelector *)Related[0]           ;
      vcb  = (VcfColorSelector *)Related[1]           ;
      vcs -> setLuv  ( LUV.L                        ) ;
      vcb -> setLuvL ( LUV.U , LUV.V , Qt::Vertical ) ;
    }                                                 ;
    break                                             ;
  }                                                   ;
  Painter . brushes [ 0 ] . setColor    ( color )     ;
  update                                (       )     ;
  if (IsNull(bundle)) return                          ;
  nIfSafe(pen     ) pen   -> setColor ( color )       ;
  nIfSafe(brush   ) brush -> setColor ( color )       ;
  nIfSafe(gradient)                                   {
    gradient -> color = color                         ;
    bundle   -> settings ( 101 )                      ;
  }                                                   ;
  bundle -> update ( )                                ;
  Alert ( Click )                                     ;
}
