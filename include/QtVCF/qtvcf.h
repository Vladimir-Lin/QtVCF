/****************************************************************************
 *                                                                          *
 * Copyright (C) 2015 Neutrino International Inc.                           *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_VCF_H
#define QT_VCF_H

#include <QtManagers>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_QTVCF_LIB)
#      define Q_VCF_EXPORT Q_DECL_EXPORT
#    else
#      define Q_VCF_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_VCF_EXPORT
#endif

namespace N
{

class Q_VCF_EXPORT VcfGlyph           ;
class Q_VCF_EXPORT VcfScreenSize      ;
class Q_VCF_EXPORT VcfPicture         ;
class Q_VCF_EXPORT VcfColorSelector   ;
class Q_VCF_EXPORT VcfColorPicker     ;
class Q_VCF_EXPORT VcfTimeBar         ;
class Q_VCF_EXPORT VcfGantt           ;
class Q_VCF_EXPORT VcfTimeSection     ;
class Q_VCF_EXPORT VcfPanel           ;
class Q_VCF_EXPORT VcfWave            ;
class Q_VCF_EXPORT VcfFlags           ;
class Q_VCF_EXPORT VcfCells           ;
class Q_VCF_EXPORT VcfParagraph       ;
class Q_VCF_EXPORT VcfBlocks          ;
class Q_VCF_EXPORT VcfMind            ;
class Q_VCF_EXPORT VcfDocument        ;
class Q_VCF_EXPORT VcfUi              ;
class Q_VCF_EXPORT VcfTask            ;
class Q_VCF_EXPORT VcfTasks           ;
class Q_VCF_EXPORT VcfAction          ;
class Q_VCF_EXPORT VcfBranch          ;
class Q_VCF_EXPORT VcfDecision        ;
class Q_VCF_EXPORT VcfView            ;
class Q_VCF_EXPORT VcfEditor          ;

namespace Graphics
{

Q_VCF_EXPORT Contours            PathToContours (QPainterPath & path) ;
Q_VCF_EXPORT Contours            Glyph          (QFont & Font,QString message) ;
Q_VCF_EXPORT QList<QPainterPath> GlyphPathes    (QFont & Font,QString message) ;

}

/*****************************************************************************
 *                                                                           *
 *                        VCF (Very Complex Format) Items                    *
 *                                                                           *
 *****************************************************************************/

class Q_VCF_EXPORT VcfGlyph : public VcfCanvas
{
  Q_OBJECT
  public:

    QString Text ;

    enum { Type = UserType + VCF::Glyph } ;
    virtual int type(void) const { return Type; }

    explicit VcfGlyph                  (VcfConstructor) ;
    virtual ~VcfGlyph                  (void);

    virtual QRectF textRect            (void) ;

  protected:

    virtual void contextMenuEvent      (QGraphicsSceneContextMenuEvent * event) ;
    virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent       * event) ;
    virtual bool ResizeFinish          (QGraphicsSceneMouseEvent       * event) ;

    virtual void Configure             (void) ;

  private:

  public slots:

    virtual void Paint                 (QPainter * painter,QRectF Region,bool clip,bool color) ;

    virtual void setText               (QString text) ;
    virtual void EnterEditor           (void) ;

    virtual void ClearGlyph            (void) ;
    virtual void GenerateGlyph         (QString text) ;

  protected slots:

    virtual void nameFinished          (void) ;
    virtual bool doubleClicked         (QPointF pos) ;
    virtual bool GlyphMenu             (QPointF pos) ;

  private slots:

  signals:

    void Append                        (VcfItem * item,VcfItem * parent) ;
    void Remove                        (VcfItem * item) ;

};

class Q_VCF_EXPORT VcfScreenSize : public VcfRectangle
{
  Q_OBJECT
  public:

    QImage   monitor ;
    QFont    font    ;
    Screen * screen  ;

    explicit VcfScreenSize        (VcfConstructor) ;
    virtual ~VcfScreenSize        (void);

    enum { Type = UserType + VCF::Calibration } ;
    virtual int type(void) const { return Type; }

    virtual void         paint    (QPainter * painter,const QStyleOptionGraphicsItem * option,QWidget * widget);
    virtual QPainterPath shape    (void) const ;

  protected:

    QList<QGraphicsSimpleTextItem *> Texts   ;

    virtual QBrush brush          (QRectF Region) ;

  private:

  public slots:

    virtual void   Paint          (QPainter * painter,QRectF Region,bool clip,bool color) ;
    virtual void   Attribute      (void) ;
    void           Park           (QSize s) ;

  protected slots:

    void           PaintImage     (QPainter * painter,QRectF Region) ;
    void           drawVertical   (QPainter * painter,QRectF Region);
    void           drawHorizontal (QPainter * painter,QRectF Region);

  private slots:

  signals:

};

class Q_VCF_EXPORT VcfPicture : public VcfRectangle
                 , public Object
{
  Q_OBJECT
  public:

    double PictureDPI ;
    QImage Image      ;
    QImage Original   ;

    enum { Type = UserType + VCF::Picture };
    virtual int type(void) const { return Type; }

    explicit VcfPicture                (VcfConstructor) ;
    virtual ~VcfPicture                (void);

    virtual void paint                 (QPainter * painter,const QStyleOptionGraphicsItem * option,QWidget * widget = 0);

    QByteArray Configuration           (void);
    bool       setConfiguration        (QByteArray & configuration);

  protected:

    virtual void contextMenuEvent      (QGraphicsSceneContextMenuEvent * event);
    virtual QVariant itemChange        (GraphicsItemChange change,const QVariant & value);
    virtual void setCornerCursor       (int corner);

    virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event);
    virtual void mouseMoveEvent        (QGraphicsSceneMouseEvent * event);
    virtual void mousePressEvent       (QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent     (QGraphicsSceneMouseEvent * event);

    virtual void MountZLevel           (QGraphicsProxyWidget * proxy,QSlider * slider);
    virtual void MountOpacity          (QGraphicsProxyWidget * proxy,QSlider * slider);
    virtual void MountRotation         (QGraphicsProxyWidget * proxy,QDial   * dial  );
    virtual void RotationUpdated       (void);
    virtual void NormalTransform       (void);
    virtual void AttachDPI             (void);
    virtual void AdjustContrast        (void) ;
    virtual void UnsharpMask           (void) ;
    virtual void SaveAs                (void) ;

  private:

    QRectF       PanelRect             (void);
    QRectF       CenterRect            (void);

  public slots:

    virtual void setCenter             (QPointF center);
    virtual void LoadImage             (SUID uuid);

    virtual void Paint                 (QPainter * painter,QRectF Region,bool clip,bool color) ;
    virtual void PaintImage            (QPainter * painter,QRectF Region,bool clip,bool color) ;
    virtual void PaintImageClip        (QPainter * painter,QRectF Region,bool clip,bool color) ;
    virtual void PaintBorder           (QPainter * painter,QRectF Region,bool clip,bool color) ;

    bool showMenu                      (QGraphicsView * view,QPoint global);

    virtual void Drag                  (void) ;
    virtual void Store                 (void) ;

    virtual void JoinColorGroup        (void) ;

  protected slots:

    void modifyDPI                     (double dpi);
    void contrastChanged               (int contrast) ;
    void usmChanged                    (int contrast) ;

  private slots:

  signals:

    void Menu                          (VcfPicture * picture,QPointF pos);
    void Delete                        (VcfPicture * picture);
    void Process                       (VcfPicture * picture);
    void Faces                         (VcfPicture * picture);
    void FacesAnalysis                 (VcfPicture * picture);
    void Store                         (VcfPicture * picture);
    void LoadSelections                (VcfPicture * picture);
    void ColorDistribution             (VcfPicture * picture);
    void TransformPicture              (VcfPicture * picture);
    void KeyPoints                     (VcfPicture * picture);
    void Channel                       (VcfPicture * picture,int Component);

};

class Q_VCF_EXPORT VcfColorSelector : public VcfPicture
{
  Q_OBJECT
  public:

    int      direction  ;
    int      colorModel ;
    QColor * color      ;

    enum { Type = UserType + VCF::Color };
    virtual int type(void) const { return Type; }

    explicit VcfColorSelector    (VcfConstructor) ;
    virtual ~VcfColorSelector    (void) ;

    virtual void paint           (QPainter * painter,const QStyleOptionGraphicsItem * option,QWidget * widget = 0);

    QColor Yuv                   (double Y,double U,double V) ;
    QColor YIQ                   (double Y,double I,double Q) ;
    QColor Lab                   (double L,double a,double b) ;
    QColor Luv                   (double L,double u,double v) ;
    QColor XYZ                   (double X,double Y,double Z) ;

  protected:

    virtual QVariant itemChange  (GraphicsItemChange change,const QVariant & value);
    virtual void mousePressEvent (QGraphicsSceneMouseEvent * event);

  private:

  public slots:

    virtual void setSize         (int width,int height) ;
    virtual void setSize         (QSize & size) ;

    // RGBA Color Model
    virtual void setR            (int direction = Qt::Vertical) ;
    virtual void setG            (int direction = Qt::Vertical) ;
    virtual void setB            (int direction = Qt::Vertical) ;
    virtual void setA            (int direction = Qt::Vertical) ;

    // CYMK Color Model
    virtual void setC            (int direction = Qt::Vertical) ;
    virtual void setY            (int direction = Qt::Vertical) ;
    virtual void setM            (int direction = Qt::Vertical) ;
    virtual void setK            (int direction = Qt::Vertical) ;

    // HSV Color Model - HS Plane
    virtual void setHSV          (double Volume) ;
    virtual void setHsvV         (int direction = Qt::Vertical) ;

    // HSL Color Model - HS Plane
    virtual void setHSL          (double Lightness) ;
    virtual void setHslL         (int direction = Qt::Vertical) ;

    // YUV Color Model - UV Plane
    virtual void setYUV          (double LumaY) ;
    virtual void setYuvY         (double U,double V,int direction = Qt::Vertical) ;

    // YIQ Color Model - IQ Plane
    virtual void setYIQ          (double LumaY) ;
    virtual void setYiqY         (double I,double Q,int direction = Qt::Vertical) ;

    // Lab Color Model - ab Plane
    virtual void setLab          (double L) ;
    virtual void setLabL         (double a,double b,int direction = Qt::Vertical) ;

    // XYZ Color Model - XY Plane
    virtual void setXYZ          (double Y) ;
    virtual void setXyzY         (double X,double Z,int direction = Qt::Vertical) ;

    // LUV Color Model - UV Plane
    virtual void setLuv          (double L) ;
    virtual void setLuvL         (double u,double v,int direction = Qt::Vertical) ;

  protected slots:

  private slots:

  signals:

    void Update                  (VcfColorSelector * selector) ;

};

class Q_VCF_EXPORT VcfColorPicker : public VcfCanvas
{
  Q_OBJECT
  public:

    int        selectMode ;
    QColor     color      ;
    Pen      * pen        ;
    Brush    * brush      ;
    Gradient * gradient   ;
    VcfItem  * bundle     ;

    enum { Type = UserType + VCF::ColorPicker };
    virtual int type(void) const { return Type; }

    explicit VcfColorPicker (VcfConstructor) ;
    virtual ~VcfColorPicker (void);

  protected:

  private:

  public slots:

    void Prepare            (void) ;
    void setCorner          (QPointF Pos) ;

    void Clear              (void) ;
    void setRgba            (void) ;
    void setCmyk            (void) ;
    void setHsv             (void) ;
    void setHsl             (void) ;
    void setYuv             (void) ;
    void setYiq             (void) ;
    void setXyz             (void) ;
    void setLab             (void) ;
    void setLuv             (void) ;

  protected slots:

    virtual void PickerMenu (VcfCanvas * canvas,QPointF pos) ;
    virtual void PickerMenu (VcfPicture * picture,QPointF pos) ;
    virtual bool PickerMenu (QPoint pos) ;

    void PickerUpdate       (VcfColorSelector * selector) ;

  private slots:

  signals:

    void Append             (VcfItem * item,VcfItem * parent) ;
    void RemoveItem         (VcfItem * item) ;

};

class Q_VCF_EXPORT VcfTimeBar : public VcfCanvas
{
  Q_OBJECT
  public:

    enum BarType     {
      Tracking = 1   ,
      Fixed    = 2 } ;

    QDateTime Start     ;
    QDateTime Final     ;
    BarType   BarMode   ;

    enum { Type = UserType + VCF::TimeBar };
    virtual int type(void) const { return Type; }

    explicit VcfTimeBar            (VcfConstructor) ;
    virtual ~VcfTimeBar            (void) ;

  protected:

    QTimer              Timer     ;
    QMap<int,QPointF  > Positions ;
    QMap<int,QDateTime> TimeLines ;

    virtual void mouseMoveEvent    (QGraphicsSceneMouseEvent * event);
    virtual void mousePressEvent   (QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event);

    virtual void Hovering          (QPointF pos);

    double       X                 (QDateTime T) ;
    QDateTime    Timeline          (double x) ;

  private:

    void         setGradient       (int id) ;

  public slots:

    virtual void Paint             (QPainter * painter,QRectF Region,bool clip,bool color) ;
    virtual void setToday          (void) ;
    virtual void Refresh           (void) ;
    virtual void Moving            (void) ;

  protected slots:

  private slots:

  signals:

    void TimeUpdated               (void)  ;

};

class Q_VCF_EXPORT VcfGantt : public VcfCanvas
{
  Q_OBJECT
  public:

    SUID      Uuid          ;
    SUID      Task          ;
    SUID      Owner         ;
    int       TextAlignment ;
    QString   User          ;
    QString   Content       ;
    QString   Note          ;
    QDateTime Start         ;
    QDateTime Final         ;
    int       EventType     ;
    int       Status        ;
    int       Adjust        ;
    QRectF    Available     ;
    QDateTime TabletStart   ;
    QDateTime TabletFinal   ;

    enum { Type = UserType + VCF::Gantt };
    virtual int type(void) const { return Type; }

    explicit VcfGantt                  (VcfConstructor) ;
    virtual ~VcfGantt                  (void) ;

    virtual QRectF ContentRect         (void) ;
    virtual QRectF EditorRect          (void) ;
    virtual QSizeF FitSize             (void) ;

    virtual QRectF  boundingRect       (void) const;
    virtual QPainterPath shape         (void) const ;

    QDateTime Timeline                 (double x) ;

  protected:

    QMap<int,QPointF  > Positions ;
    QMap<int,QDateTime> TimeLines ;
    NAMEs               TypeNames ;

    virtual void Configure             (void) ;

    virtual void hoverEnterEvent       (QGraphicsSceneHoverEvent * event);
    virtual void hoverLeaveEvent       (QGraphicsSceneHoverEvent * event);
    virtual void hoverMoveEvent        (QGraphicsSceneHoverEvent * event);
    virtual void Hovering              (QPointF pos);

    virtual void contextMenuEvent      (QGraphicsSceneContextMenuEvent * event) ;
    virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event) ;
    virtual void mouseMoveEvent        (QGraphicsSceneMouseEvent * event);
    virtual void mousePressEvent       (QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent     (QGraphicsSceneMouseEvent * event);

    virtual bool CursorMoving          (QGraphicsSceneMouseEvent * event) ;
    virtual bool ResizeStart           (QGraphicsSceneMouseEvent * event) ;
    virtual bool ResizeMoving          (QGraphicsSceneMouseEvent * event) ;
    virtual bool ResizeFinish          (QGraphicsSceneMouseEvent * event) ;

    virtual void DurationChanged       (void) ;

    virtual bool dropText              (QWidget * source,QPointF pos,const QString & text) ;
    virtual bool dropUrls              (QWidget * source,QPointF pos,const QList<QUrl> & urls) ;
    virtual bool dropHtml              (QWidget * source,QPointF pos,const QString & html) ;
    virtual bool dropColor             (QWidget * source,QPointF pos,const QColor  & color) ;
    virtual bool dropPeople            (QWidget * source,QPointF pos,const UUIDs   & Uuids) ;
    virtual bool dropVideos            (QWidget * source,QPointF pos,const UUIDs   & Uuids) ;
    virtual bool dropAlbums            (QWidget * source,QPointF pos,const UUIDs   & Uuids) ;
    virtual bool dropURIs              (QWidget * source,QPointF pos,const UUIDs   & Uuids) ;
    virtual bool dropBookmarks         (QWidget * source,QPointF pos,const UUIDs   & Uuids) ;
    virtual bool dropFont              (QWidget * source,QPointF pos,const SUID      font) ;
    virtual bool dropPen               (QWidget * source,QPointF pos,const SUID      pen) ;
    virtual bool dropBrush             (QWidget * source,QPointF pos,const SUID      brush) ;
    virtual bool dropGradient          (QWidget * source,QPointF pos,const SUID      gradient) ;

  private:

  public slots:

    virtual void Paint                 (QPainter * painter,QRectF Region,bool clip,bool color) ;
    virtual void PaintText             (QPainter * painter,QRectF Region,bool clip,bool color);
    virtual void PaintNone             (QPainter * painter,QRectF Region,bool clip,bool color);
    virtual void PaintShape            (QPainter * painter,QRectF Region,bool clip,bool color);
    virtual void PaintSleep            (QPainter * painter,QRectF Region,bool clip,bool color);

    virtual void setText               (QString text);
    virtual void MountEditor           (void) ;
    virtual void GanttUpdated          (void) ;
    virtual void GanttMoving           (void) ;
    virtual void DropChanged           (void) ;

  protected slots:

    virtual void NameFinished          (void) ;
    virtual bool GanttMenu             (QPointF pos) ;

    void         ClearExecution        (void) ;
    void         MountExecution        (void) ;
    void         ExecutionChanged      (int index) ;

  private slots:

  signals:

    void Changed                       (VcfGantt * gantt) ;
    void Updated                       (VcfGantt * gantt) ;
    void Edit                          (VcfGantt * gantt,int Id) ;

};

class Q_VCF_EXPORT VcfTimeSection : public VcfCanvas
{
  Q_OBJECT
  public:

    QDateTime Start     ;
    QDateTime Final     ;

    enum { Type = UserType + VCF::TimeSection };
    virtual int type(void) const { return Type; }

    explicit VcfTimeSection        (VcfConstructor) ;
    virtual ~VcfTimeSection        (void) ;

    double       X                 (QDateTime T) ;
    QDateTime    Timeline          (double x) ;

  protected:

    VcfGantt          * Gantt     ;
    QMap<int,QPointF  > Positions ;
    QMap<int,QDateTime> TimeLines ;

    virtual void mouseMoveEvent    (QGraphicsSceneMouseEvent * event);
    virtual void mousePressEvent   (QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event);

    virtual void Hovering          (QPointF pos);

    bool         isGantt           (void) ;

  private:

  public slots:

    virtual void Paint             (QPainter * painter,QRectF Region,bool clip,bool color) ;

    void GanttStart                (void) ;
    void GanttMoving               (void) ;
    void GanttFinished             (void) ;

  protected slots:

  private slots:

  signals:

    void Append                    (QDateTime Start,QDateTime Final) ;

};

class Q_VCF_EXPORT VcfPanel : public VcfCanvas
{
  Q_OBJECT
  public:

    enum { Type = UserType + VCF::Panel };
    virtual int type(void) const { return Type; }

    explicit VcfPanel             (VcfConstructor) ;
    virtual ~VcfPanel             (void) ;

    QList<QAction *> actions      (void) ;
    QToolBar *       ToolBar      (void) ;

  protected:

    QToolBar             * Tool      ;
    QGraphicsProxyWidget * ToolProxy ;

    virtual void contextMenuEvent (QGraphicsSceneContextMenuEvent * event);

  private:

  public slots:

    virtual void Attach           (QToolBar * toolBar = NULL) ;
    virtual void setRange         (QRectF rect) ;

  protected slots:

    virtual bool Menu             (QPointF pos) ;

  private slots:

  signals:

};

class Q_VCF_EXPORT VcfWave : public VcfCanvas
{
  Q_OBJECT
  public:

    bool DrawDot     ;
    bool DrawLine    ;
    bool PrivateMenu ;
    bool HighQuality ;

    enum { Type = UserType + VCF::Wave };
    virtual int type(void) const { return Type; }

    explicit VcfWave              (VcfConstructor) ;
    virtual ~VcfWave              (void) ;

  protected:

    int      Total           ;
    double * Values          ;
    double   Shift           ;
    double   Factor          ;
    QList<QPointF> Points    ;

    virtual void Configure        (void) ;
    virtual void Hovering         (QPointF pos) ;
    virtual void ResizeRect       (QPointF P1,QPointF P2);
    virtual void ResizeWidth      (QPointF P1,QPointF P2);
    virtual void ResizeHeight     (QPointF P1,QPointF P2) ;
    virtual bool ResizeStart      (QGraphicsSceneMouseEvent * event) ;
    virtual bool ResizeFinish     (QGraphicsSceneMouseEvent * event) ;
    virtual void contextMenuEvent (QGraphicsSceneContextMenuEvent * event) ;
    QVariant itemChange           (GraphicsItemChange change,const QVariant & value) ;

    virtual void updateGeometry   (void) ;

    virtual double Amplitude      (int index) ;

  private:

  public slots:

    virtual void Paint            (QPainter * painter,QRectF Region,bool clip,bool color) ;

    virtual void setRange         (QRectF paper) ;
    virtual void setData          (int total,double * values) ;
    virtual void setFactor        (double shift,double factor) ;
    virtual void updateWave       (void) ;
    virtual void Empty            (double zero) ;
    virtual void Empty            (int tail,double zero) ;

  protected slots:

    virtual bool Menu             (QPointF pos) ;

  private slots:

  signals:

    void Menu                     (VcfWave * canvas,QPointF pos);

};

class Q_VCF_EXPORT VcfFlags : public VcfInterface
{
  Q_OBJECT
  public:

    QGraphicsItem * Item ;

    enum { Type = UserType + VCF::Flags };
    virtual int type(void) const { return Type; }

    explicit VcfFlags  (VcfConstructor) ;
    virtual ~VcfFlags  (void) ;

  protected:

    QTreeWidget * Flags ;

    void AddItem       (QString name,GraphicsItemFlag flag) ;

  private:

  public slots:

    bool Initialize    (QGraphicsItem * item,QRectF rect) ;
    void List          (void) ;

  protected slots:

    void doubleClicked (QTreeWidgetItem * item,int column) ;

  private slots:

  signals:

};

class Q_VCF_EXPORT VcfCells : public VcfNode
{
  Q_OBJECT
  public:

    enum { Type = UserType + VCF::Cells };
    virtual int type(void) const { return Type; }

    explicit VcfCells             (VcfConstructor) ;
    virtual ~VcfCells             (void) ;

    virtual  QString Value        (int row,int column) ;
    virtual  QRectF  Cell         (int row,int column) ;

    virtual  int columnCount      (void) ;
    virtual  int rowCount         (void) ;

  protected:

    QSize Dimension ;

    virtual void Configure        (void) ;

  private:

  public slots:

    virtual void RenderAppearance (QPainter * painter,QRectF Region,bool clip,bool color) ;
    virtual void RenderObjects    (QPainter * painter,QRectF Region,bool clip,bool color) ;
    virtual void Paint            (QPainter * painter,int row,int column) ;

    virtual void setColumnCount   (int column) ;
    virtual void setRowCount      (int row) ;

  protected slots:

  private slots:

  signals:

};

class Q_VCF_EXPORT VcfParagraph : public VcfBlock
{
  Q_OBJECT
  public:

    TextDocument        *  Document   ;
    QList<QRectF>          Composes   ;
    QPainterPath           Available  ;
    QMap<VcfItem *,QRectF> Exclusions ;

    enum { Type = UserType + VCF::Paragraph };
    virtual int type(void) const { return Type; }

    explicit VcfParagraph              (VcfConstructor) ;
    virtual ~VcfParagraph              (void) ;

    virtual void paint                 (QPainter * painter,const QStyleOptionGraphicsItem * option,QWidget * widget = 0);

    QPainterPath visiblePath           (void) ;
    QString      tabReplacement        (void) ;

  protected:

    VcfCursor          *  cursor         ;
    TextCursor            textCursor     ;
    TextCursor            arrangement    ;
    QString               TabReplacement ;
    QMutex                locator        ;
    Qt::KeyboardModifiers modifiers      ;

    virtual QVariant itemChange        (GraphicsItemChange change,const QVariant & value);

    virtual void contextMenuEvent      (QGraphicsSceneContextMenuEvent * event);
    virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event);

    virtual void mousePressEvent       (QGraphicsSceneMouseEvent * event);
    virtual void mouseMoveEvent        (QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent     (QGraphicsSceneMouseEvent * event);

    virtual void     inputMethodEvent  (QInputMethodEvent * event);
    virtual QVariant inputMethodQuery  (Qt::InputMethodQuery query) const;

    virtual bool sceneEvent            (QEvent * event) ;
    virtual void keyPressEvent         (QKeyEvent * event) ;
    virtual void keyReleaseEvent       (QKeyEvent * event) ;

    virtual void Configure             (void) ;
    virtual void Hovering              (QPointF pos);

    virtual QRectF SectionRect         (QRectF rect) ;

    virtual void setDefaultSection     (TextSection & NTS) ;
    bool         isModifier            (enum Qt::KeyboardModifier modifier) ;
    bool         isCursorEditing       (void) ;
    QPointF      CursorBottom          (QPointF pos) ;
    int          atPage                (QPointF pos) ;

  private:

  public slots:

    virtual void Paint                 (QPainter * painter,QRectF Region,bool clip,bool color) ;
    virtual void RenderAll             (QPainter * painter) ;

    virtual bool Menu                  (QPointF pos) ;
    virtual void setTabReplacement     (QString tab) ;

    virtual bool FocusIn               (void) ;
    virtual bool FocusOut              (void) ;

    virtual void CursorPressed         (void) ;
    virtual void CursorMoving          (void) ;
    virtual void CursorReleased        (void) ;
    virtual void CursorCombined        (void) ;
    virtual void LocateCursor          (void) ;
    virtual void setTextCursor         (QPointF pos) ;
    virtual void setCursorPosition     (QRectF Range) ;
    virtual int  addPaper              (QRectF Paper) ;

    virtual void Prepare               (void) ;
    virtual void BeginPartition        (TextDocument * document) ;
    virtual int  Partition             (void) ;
    virtual bool ArrangeBlock          (void) ;
    virtual void Arrange               (void) ;
    virtual void EndPartition          (void) ;

    virtual void CursorLeft            (void) ;
    virtual void CursorRight           (void) ;
    virtual void CursorUp              (void) ;
    virtual void CursorDown            (void) ;
    virtual void CursorHome            (void) ;
    virtual void CursorEnd             (void) ;
    virtual void CursorDelete          (void) ;
    virtual void CursorEnter           (void) ;
    virtual void CursorBackspace       (void) ;
    virtual void CursorTab             (void) ;

    virtual void ShiftLeft             (void) ;
    virtual void ShiftRight            (void) ;
    virtual void ShiftUp               (void) ;
    virtual void ShiftDown             (void) ;
    virtual void ShiftHome             (void) ;
    virtual void ShiftEnd              (void) ;
    virtual void ShiftTab              (void) ;

    virtual void CtrlBackspace         (void) ;

    virtual void CopyToClipboard       (void) ;
    virtual void Paste                 (void) ;
    virtual void Cut                   (void) ;
    virtual void SelectAll             (void) ;
    virtual void LocateAnchor          (void) ;
    virtual void ClearSelection        (void) ;

    virtual void clearExclusions       (void) ;
    virtual void addExclusions         (VcfItem * item,QRectF itemRect) ;

    virtual bool insertText            (QString text) ;
    virtual bool insertIM              (QString commit,QString preedit) ;

  protected slots:

  private slots:

  signals:

    void ParagraphMenu                 (VcfParagraph * paragraph,QPointF pos) ;
    void scrollUp                      (VcfParagraph * paragraph,QPointF pos) ;
    void scrollDown                    (VcfParagraph * paragraph,QPointF pos) ;
    void acquirePaper                  (VcfParagraph * paragraph) ;
    void shrinkPaper                   (VcfParagraph * paragraph) ;

};

class Q_VCF_EXPORT VcfBlocks : public VcfBlock
                , public Graph
{
  Q_OBJECT
  public:

    QSizeF   alignGrid ;
    VcfNodes children  ; // make some efforts to kill this usage
    bool     expanded  ;

    enum { Type = UserType + VCF::Blocks };
    virtual int type(void) const { return Type; }

    explicit VcfBlocks                (VcfConstructor) ;
    virtual ~VcfBlocks                (void) ;

    virtual VcfGrid   * CreateGrid    (QRectF Range) ;
    virtual VcfBlock  * CreateBlock   (QRectF Range) ;
    virtual VcfLinker * CreateLinker  (QPointF start,QPointF final) ;
    virtual VcfLinker * CreateLinker  (VcfNode * node,QPointF start,QPointF final) ;

    QPointF   alignPos                (QPointF pos) ;

  protected:

    VcfLinker * attemptLink ;

    virtual void Configure            (void) ;
    virtual void Hovering             (QPointF pos);
    virtual enum EditWays CastEditing (void) ;

  private:

  public slots:

    virtual void addChildren          (VcfNode * node);

    virtual void setRange             (QRectF rect) ;
    virtual void ConnectMenu          (VcfNode * node,QPointF ips,QPointF pos,MenuManager & menu) ;

    virtual void startConnection      (VcfNode * node,QPointF pos,int flags) ;
    virtual void moveConnection       (QPointF pos) ;
    virtual void endConnection        (QPointF pos) ;

  protected slots:

    virtual void BlockUpdated         (VcfBlock * block,int item) ;

  private slots:

  signals:

};

class Q_VCF_EXPORT VcfMind : public VcfBlocks
{
  Q_OBJECT
  public:

    enum { Type = UserType + VCF::Mind };
    virtual int type(void) const { return Type; }

    explicit VcfMind                  (VcfConstructor) ;
    virtual ~VcfMind                  (void) ;

  protected:

    virtual void Configure            (void) ;
    virtual enum EditWays CastEditing (void) ;

    virtual void connectPressEvent    (QGraphicsSceneMouseEvent * event) ;
    virtual void connectMoveEvent     (QGraphicsSceneMouseEvent * event) ;
    virtual void connectReleaseEvent  (QGraphicsSceneMouseEvent * event) ;

  private:

  public slots:

    virtual void setRange             (QRectF rect) ;
    virtual void Initialize           (void) ;

    virtual bool FocusIn              (void) ;
    virtual bool FocusOut             (void) ;

    virtual void EditMenu             (MenuManager & menu,QPointF pos) ;
    virtual void ConnectMenu          (VcfNode * node,QPointF ips,QPointF pos,MenuManager & menu) ;
    virtual bool Menu                 (QPointF pos) ;
    virtual bool MenuProcess          (QAction * action,MenuManager & menu) ;

    virtual bool doubleClicked        (QPointF pos) ;

  protected slots:

    virtual void BlockUpdated         (VcfBlock * block,int item) ;
    virtual void BlockMenu            (VcfNode * node,QPointF pos) ;

  private slots:

  signals:

};

class Q_VCF_EXPORT VcfDocument : public VcfBlocks
{
  Q_OBJECT
  public:

    enum { Type = UserType + VCF::Document };
    virtual int type(void) const { return Type; }

    explicit VcfDocument (VcfConstructor) ;
    virtual ~VcfDocument (void) ;

  protected:

  private:

  public slots:

    virtual void Paint   (QPainter * painter,QRectF Region,bool clip,bool color) ;

  protected slots:

  private slots:

  signals:

};

class Q_VCF_EXPORT VcfUi : public VcfBlocks
{
  Q_OBJECT
  public:

    enum { Type = UserType + VCF::UI };
    virtual int type(void) const { return Type; }

    explicit VcfUi     (VcfConstructor) ;
    virtual ~VcfUi     (void) ;

  protected:

  private:

  public slots:

    virtual void Paint (QPainter * painter,QRectF Region,bool clip,bool color) ;

  protected slots:

  private slots:

  signals:

};

class Q_VCF_EXPORT VcfTask : public VcfBlocks
{
  Q_OBJECT
  public:

    enum { Type = UserType + VCF::Task };
    virtual int type(void) const { return Type; }

    explicit VcfTask              (VcfConstructor) ;
    virtual ~VcfTask              (void) ;

    QPointF  Baseline             (void) ;

    virtual bool AttemptMelt      (VcfLinker * Linker,QPointF base) ;
    virtual bool AllowMelts       (int side) ;

  protected:

    QRectF iconRect ;

    virtual void Configure        (void) ;
    virtual void Hovering         (QPointF pos);

  private:

  public slots:

    virtual void Arrange          (void) ;

    virtual bool FocusIn          (void) ;
    virtual bool FocusOut         (void) ;

    virtual void Paint            (QPainter * painter,QRectF Region,bool clip,bool color) ;
    virtual void RenderIcon       (QPainter * painter,QRectF Region,bool clip,bool color) ;
    virtual void RenderAppearance (QPainter * painter,QRectF Region,bool clip,bool color) ;

  protected slots:

  private slots:

  signals:

};

class Q_VCF_EXPORT VcfTasks : public VcfBlocks
               , public TaskManager
{
  Q_OBJECT
  public:

    enum { Type = UserType + VCF::Tasks };
    virtual int type(void) const { return Type; }

    explicit       VcfTasks       (VcfConstructor) ;
    virtual       ~VcfTasks       (void) ;

    VcfTask *      CreateTask     (VcfNode * Parent,QRectF Range,SUID uuid,int nodeType) ;

  protected:

    virtual void   Configure      (void) ;
    virtual void   Hovering       (QPointF pos);

  private:

  public slots:

    virtual void   Paint          (QPainter * painter,QRectF Region,bool clip,bool color) ;

    virtual void   Initialize     (SUID uuid = 0) ;
    virtual double addTopLevel    (SUID uuid,QPointF position) ;
    virtual void   addFramework   (VcfTask * task) ;
    virtual void   joinFramework  (VcfTask * task,SUID uuid);
    virtual void   addSuccessor   (VcfTask * task,VcfTask * predecessor) ;
    virtual void   MountLinker    (VcfLinker * linker,VcfTask * first,VcfTask * second) ;
    virtual void   MeltLinker     (VcfLinker * linker,QPointF pz,QPointF pe) ;

  protected slots:

    virtual bool   Menu           (QPointF pos) ;
    virtual void   TaskMenu       (VcfNode * node,QPointF pos) ;

    virtual void   AcceptDissolve (VcfItem   * item) ;
    virtual void   AcceptMoving   (VcfItem   * item) ;

  private slots:

  signals:

};

class Q_VCF_EXPORT VcfAction : public VcfBlock
{
  Q_OBJECT
  public:

    enum { Type = UserType + VCF::Action };
    virtual int type(void) const { return Type; }

    explicit     VcfAction (VcfConstructor) ;
    virtual     ~VcfAction (void) ;

  protected:

    virtual void Configure (void) ;

  private:

  public slots:

    virtual void Paint     (QPainter * painter,QRectF Region,bool clip,bool color) ;

  protected slots:

  private slots:

  signals:

};

class Q_VCF_EXPORT VcfBranch : public VcfBlock
{
  Q_OBJECT
  public:

    enum { Type = UserType + VCF::Branch };
    virtual int type(void) const { return Type; }

    explicit     VcfBranch (VcfConstructor) ;
    virtual     ~VcfBranch (void) ;

  protected:

    virtual void Configure (void) ;

  private:

  public slots:

    virtual void Paint     (QPainter * painter,QRectF Region,bool clip,bool color) ;

  protected slots:

  private slots:

  signals:

};

class Q_VCF_EXPORT VcfDecision : public VcfBlocks
{
  Q_OBJECT
  public:

    enum { Type = UserType + VCF::Decision };
    virtual int type(void) const { return Type; }

    explicit     VcfDecision   (VcfConstructor) ;
    virtual     ~VcfDecision   (void) ;

  protected:

    virtual void Configure     (void) ;
    virtual void Hovering      (QPointF pos);

  private:

  public slots:

    virtual void Paint         (QPainter * painter,QRectF Region,bool clip,bool color) ;

    virtual bool FocusIn       (void) ;
    virtual bool FocusOut      (void) ;

    virtual bool doubleClicked (QPointF pos) ;
    virtual bool Menu          (QPointF pos) ;
    virtual void DecisionMenu  (MenuManager & menu,QPointF pos) ;

    virtual void setUuid       (QString name,SUID uuid) ;

  protected slots:

  private slots:

  signals:

};

/*****************************************************************************
 *                                                                           *
 *                               VCF GraphicsView                            *
 *                                                                           *
 *****************************************************************************/

class Q_VCF_EXPORT VcfView : public GraphicsView
{
  Q_OBJECT
  public:

    int  Mode      ;
    int  Method    ;
    SUID Group     ;
    SUID Uuid      ;
    bool skipMouse ;

    explicit             VcfView           (StandardConstructor) ;
    virtual             ~VcfView           (void);

    bool                 validPicture      (QFileInfo info) ;

    VcfCanvas         *  NewCanvas         (int mode,VcfItem * vcf) ;
    VcfCanvas         *  NewCanvas         (VcfItem * vcf) ;
    VcfPaper          *  NewPaper          (VcfItem * vcf) ;
    VcfLabel          *  NewLabel          (VcfItem * vcf,QRectF paper,QString content);
    VcfColorPicker    *  NewColorPicker    (VcfItem * vcf) ;
    VcfInterface      *  NewInterface      (VcfItem * vcf) ;

    VcfPicture        *  NewPicture        (VcfItem * vcf) ;
    VcfPicture        *  NewPicture        (VcfItem * vcf,SUID uuid) ;
    VcfPicture        *  NewPicture        (VcfItem * vcf,QImage image) ;

    VcfPicture        *  CropPicture       (VcfPicture * picture,VcfCanvas * canvas) ;

    VcfPicture        *  CannyPicture      (VcfPicture * picture,VcfCanvas * canvas) ;
    VcfPicture        *  LaplacePicture    (VcfPicture * picture,VcfCanvas * canvas) ;

    VcfPicture        *  SobelPicture      (VcfPicture * picture,VcfCanvas * canvas) ;
    VcfPicture        *  VerticalSobel     (VcfPicture * picture,VcfCanvas * canvas) ;
    VcfPicture        *  HorizontalSobel   (VcfPicture * picture,VcfCanvas * canvas) ;

    QList<VcfContours *> SobelDetection    (VcfPicture * picture,VcfCanvas * canvas) ;
    QList<VcfContours *> CannyDetection    (VcfPicture * picture,VcfCanvas * canvas) ;
    QList<VcfContours *> LaplaceDetection  (VcfPicture * picture,VcfCanvas * canvas) ;

    virtual QList<VcfContours *> ContoursDetection (VcfPicture * picture,VcfCanvas * canvas) ;

  protected:

    virtual bool focusNextPrevChild    (bool next                ) ;
    virtual void focusInEvent          (QFocusEvent       * event) ;
    virtual void focusOutEvent         (QFocusEvent       * event) ;

    virtual void dragEnterEvent        (QDragEnterEvent   * event) ;
    virtual void dragLeaveEvent        (QDragLeaveEvent   * event) ;
    virtual void dragMoveEvent         (QDragMoveEvent    * event) ;
    virtual void dropEvent             (QDropEvent        * event) ;

    virtual void inputMethodEvent      (QInputMethodEvent * event) ;
    virtual void keyPressEvent         (QKeyEvent         * event) ;
    virtual void keyReleaseEvent       (QKeyEvent         * event) ;
    virtual void contextMenuEvent      (QContextMenuEvent * event) ;
    virtual void mousePressEvent       (QMouseEvent       * event) ;
    virtual void mouseReleaseEvent     (QMouseEvent       * event) ;
    virtual void mouseMoveEvent        (QMouseEvent       * event) ;
    virtual void mouseDoubleClickEvent (QMouseEvent       * event) ;
    virtual void wheelEvent            (QWheelEvent       * event) ;

    virtual void resizeEvent           (QResizeEvent      * event) ;

    virtual void drawBackground        (QPainter * painter, const QRectF & rect) ;

    virtual bool FocusIn               (void);

    virtual bool Relocation            (void);

    void         PerfectView           (void);

  private:

  public slots:

    #ifdef QT_OPENGL_LIB
    virtual void setupOpenGL           (void) ;
    virtual void Rendering             (void) ;
    #endif

    virtual void AdjustMdi             (QMdiArea * mdi) ;

    virtual void DefaultZooms          (void) ;
    virtual void ZoomIn                (void) ;
    virtual void ZoomOut               (void) ;
    virtual void Zoom                  (void) ;
    virtual void UnitedZoom            (void) ;
    virtual void PaperZoom             (void) ;

    virtual void AddPaper              (void) ;

    virtual void SolitaryPicture       (VcfPicture * picture,double zValue) ;
    virtual void PutPicture            (VcfPicture * picture,double zValue) ;
    virtual void ShowPicture           (VcfPicture * picture) ;

    virtual bool LoadPicture           (SUID uuid) ;
    virtual bool LoadPicture           (QFileInfo info) ;
    virtual void ViewPicture           (SUID uuid,MdiArea * Mdi) ;
    virtual void ViewPicture           (QFileInfo info,MdiArea * Mdi) ;
    virtual void ViewPicture           (QImage image,MdiArea * Mdi) ;
    virtual void KeyPoints             (VcfPicture * picture) ;
    virtual void SaveSelection         (VcfPicture * picture,VcfCanvas * canvas) ;
    virtual void RemoveSelection       (VcfPicture * picture,VcfCanvas * canvas) ;

    virtual void EditPenColor          (VcfCanvas * canvas) ;
    virtual void EditBrushColor        (VcfCanvas * canvas) ;
    virtual void EditGradient          (VcfCanvas * canvas) ;

    virtual void EditFlags             (QGraphicsItem * item) ;

  protected slots:

    virtual bool Menu                  (QPoint pos);
    virtual void CanvasMenu            (VcfCanvas   * canvas ,QPointF pos ) ;
    virtual void ShapeName             (VcfCanvas   * canvas ,QString name) ;
    virtual void PictureMenu           (VcfPicture  * picture,QPointF pos ) ;
    virtual void ContourMenu           (VcfContours * contour,QPointF pos ) ;
    virtual void DeletePicture         (VcfPicture  * picture) ;
    virtual void StorePicture          (VcfPicture  * picture) ;
    virtual void PictureProcess        (VcfPicture  * picture) ;
    virtual void PictureFaces          (VcfPicture  * picture) ;
    virtual void FacesAnalysis         (VcfPicture  * picture) ;
    virtual void PictureSelections     (VcfPicture  * picture) ;
    virtual void ColorDistribution     (VcfPicture  * picture) ;
    virtual void TransformPicture      (VcfPicture  * picture) ;
    virtual void PictureChannel        (VcfPicture  * picture,int Component) ;
    virtual void Selection             (VcfItem     * item,QRectF Screen) ;
    virtual void Canvas                (VcfItem     * item,QRectF Screen) ;
    virtual void StoreContours         (VcfCanvas   * canvas,VcfContours * contour) ;

  private slots:

  signals:

    void Store                         (int method,SUID group,SUID uuid) ;

};

class Q_VCF_EXPORT VcfEditor : public GraphicsView
{
  Q_OBJECT
  public:

    enum VcfMode              {
      FreeMode         =  0   ,
      PlainMode        =  1   ,
      DocumentMode     =  2   ,
      KnowledgeMode    =  3   ,
      FlowMode         =  4   ,
      SqlMode          =  5   ,
      DecisionMode     =  6   ,
      TaskMode         =  7   ,
      GenealogicalMode =  8   ,
      MindMode         =  9 } ;

    int     Mode       ;
    int     Method     ;
    SUID    Group      ;
    SUID    Uuid       ; // VCF Root
    int     SourceType ;
    QString Version    ;
    QString Title      ;
    BMAPs   Booleans   ;

    explicit             VcfEditor          (StandardConstructor) ;
    virtual             ~VcfEditor          (void);

    bool                 validPicture      (QFileInfo info) ;

    VcfCanvas         *  NewCanvas         (int mode,VcfItem * vcf) ;
    VcfCanvas         *  NewCanvas         (VcfItem * vcf) ;
    VcfPaper          *  NewPaper          (VcfItem * vcf) ;
    VcfLabel          *  NewLabel          (VcfItem * vcf,QRectF paper,QString content);
    VcfColorPicker    *  NewColorPicker    (VcfItem * vcf) ;
    VcfInterface      *  NewInterface      (VcfItem * vcf) ;

    VcfPicture        *  NewPicture        (VcfItem * vcf) ;
    VcfPicture        *  NewPicture        (VcfItem * vcf,SUID uuid) ;
    VcfPicture        *  NewPicture        (VcfItem * vcf,QImage image) ;

    VcfPicture        *  CropPicture       (VcfPicture * picture,VcfCanvas * canvas) ;

    VcfPicture        *  CannyPicture      (VcfPicture * picture,VcfCanvas * canvas) ;
    VcfPicture        *  SobelPicture      (VcfPicture * picture,VcfCanvas * canvas) ;
    VcfPicture        *  LaplacePicture    (VcfPicture * picture,VcfCanvas * canvas) ;

    QList<VcfContours *> SobelDetection    (VcfPicture * picture,VcfCanvas * canvas) ;
    QList<VcfContours *> CannyDetection    (VcfPicture * picture,VcfCanvas * canvas) ;
    QList<VcfContours *> LaplaceDetection  (VcfPicture * picture,VcfCanvas * canvas) ;

    virtual QList<VcfContours *> ContoursDetection (VcfPicture * picture,VcfCanvas * canvas) ;

    virtual bool isRunning                 (void) ;

    int          FindChannel        (void) ;
    virtual bool CreateChannel      (int Channel) ;
    virtual void OptimizeChannel    (int Channel,unsigned long FramesPerBuffer) ;
    virtual void ClearChannel       (int Channel) ;
    virtual void FeedAudio          (int Channel,      void * Output,unsigned long FramesPerBuffer) ;
    virtual void RecordAudio        (int Channel,const void * Input ,unsigned long FramesPerBuffer) ;
    virtual bool OutputChannel      (int Channel,int DeviceId,int BufferSize,int Channels,int SampleFormat,int SampleRate) ;
    virtual bool InputChannel       (int Channel,int DeviceId,int BufferSize,int Channels,int SampleFormat,int SampleRate) ;

    virtual void RunStream          (int stream) ;
    virtual void DestroyStream      (int stream) ;

  protected:

    enum WidgetIDs   {
      veMdi    = 0   ,
      veCursor = 1   ,
      veZoom   = 2   ,
      vePage   = 3 } ;

    UUIDs                      PlainUuids      ;
    QMap<SUID,TextDocument *>  Documents       ;
    QMap<int,QWidget *>        Widgets         ;
    QTimer                     AutoSave        ;
    QTimer                     Timer           ;
    QMutex                     TimerLock       ;
    VarArgLists                Commands        ;
    QPoint                     dragPoint       ;
    QMap<int,void *>           AudioChannels   ;
    QMap<int,void *>           AudioFeeders    ;
    QMap<int,void *>           StreamChannels  ;
    QMap<int,void *>           DisplayChannels ;
    QMap<int,int   >           OutputMaps      ;
    QMap<int,int   >           OutputTypes     ;

    virtual void drawBackground        (QPainter * painter,const QRectF & rect) ;
    virtual void drawForeground        (QPainter * painter,const QRectF & rect) ;

    virtual void enterEvent            (QEvent            * event) ;
    virtual void leaveEvent            (QEvent            * event) ;
    virtual void paintEvent            (QPaintEvent       * event) ;
    virtual bool focusNextPrevChild    (bool next                ) ;
    virtual void focusInEvent          (QFocusEvent       * event) ;
    virtual void focusOutEvent         (QFocusEvent       * event) ;
    virtual void dragEnterEvent        (QDragEnterEvent   * event) ;
    virtual void dragLeaveEvent        (QDragLeaveEvent   * event) ;
    virtual void dragMoveEvent         (QDragMoveEvent    * event) ;
    virtual void dropEvent             (QDropEvent        * event) ;
    virtual void inputMethodEvent      (QInputMethodEvent * event) ;
    virtual void keyPressEvent         (QKeyEvent         * event) ;
    virtual void keyReleaseEvent       (QKeyEvent         * event) ;
    virtual void contextMenuEvent      (QContextMenuEvent * event) ;
    virtual void mousePressEvent       (QMouseEvent       * event) ;
    virtual void mouseReleaseEvent     (QMouseEvent       * event) ;
    virtual void mouseMoveEvent        (QMouseEvent       * event) ;
    virtual void mouseDoubleClickEvent (QMouseEvent       * event) ;
    virtual void wheelEvent            (QWheelEvent       * event) ;
    virtual void resizeEvent           (QResizeEvent      * event) ;
    virtual bool event                 (QEvent            * event) ;

    virtual void Configure             (void) ;
    virtual bool FocusIn               (void) ;
    virtual bool FocusOut              (void) ;
    virtual bool Relocation            (void) ;
    void         PerfectView           (void) ;

    QMdiSubWindow * GetSubWindow       (void) ;
    QLabel        * GetLabel           (int id) ;

    VcfParagraph * addPlainText        (int StartPage,TextDocument * document) ;

    QString        CursorPosition      (QPointF paper) ;

    virtual void   FeedChannel         (int channel,int feeder,void * output) ;

  private:

  public slots:

    virtual void AdjustMdi             (QMdiArea * mdi) ;
    virtual bool Shutdown              (void) ;

    virtual void Prepare               (void) ;
    virtual void setModified           (bool modified) ;
    virtual void setOriginate          (SUID uuid,int sourceType) ;
    virtual void setTitle              (QString title,QString version) ;
    virtual bool loadVcf               (SUID uuid,int mode) ;
    virtual bool loadPlainText         (SUID fileuuid);

    virtual void DefaultZooms          (void) ;
    virtual void ZoomIn                (void) ;
    virtual void ZoomOut               (void) ;
    virtual void Zoom                  (void) ;
    virtual void UnitedZoom            (void) ;
    virtual void PaperZoom             (void) ;

    virtual void AddPaper              (void) ;

    virtual void SolitaryPicture       (VcfPicture * picture,double zValue) ;
    virtual void PutPicture            (VcfPicture * picture,double zValue) ;
    virtual void ShowPicture           (VcfPicture * picture) ;

    virtual bool LoadPicture           (SUID uuid) ;
    virtual bool LoadPicture           (QFileInfo info) ;
    virtual void ViewPicture           (SUID uuid,MdiArea * Mdi) ;
    virtual void ViewPicture           (QFileInfo info,MdiArea * Mdi) ;
    virtual void SaveSelection         (VcfPicture * picture,VcfCanvas * canvas) ;
    virtual void RemoveSelection       (VcfPicture * picture,VcfCanvas * canvas) ;

    virtual void EditPenColor          (VcfCanvas * canvas) ;
    virtual void EditBrushColor        (VcfCanvas * canvas) ;
    virtual void EditGradient          (VcfCanvas * canvas) ;

    virtual void EditFlags             (QGraphicsItem * item) ;

    virtual void scrollUp              (VcfParagraph * paragraph,QPointF pos) ;
    virtual void scrollDown            (VcfParagraph * paragraph,QPointF pos) ;
    virtual void acquirePaper          (VcfParagraph * paragraph) ;
    virtual void shrinkPaper           (VcfParagraph * paragraph) ;

    virtual void Insert                (void) ;
    virtual void Print                 (void) ;
    virtual void ExportPDF             (void) ;
    virtual void Export                (void) ;
    virtual void Load                  (void) ;
    virtual void Save                  (void) ;
    virtual void SaveAs                (void) ;
    virtual void Close                 (void) ;
    virtual void Undo                  (void) ;
    virtual void Redo                  (void) ;
    virtual void Select                (void) ;
    virtual void Properties            (void) ;

    virtual bool Interrupt             (void) ;

    virtual bool StopOutput            (int Channel) ;

    virtual bool PlayAudioByFeeder     (SUID uuid) ;
    virtual bool PlayAudioByFeeder     (int channel,QString filename) ;
    virtual bool LoadAudioToFeeder     (int channel,QString filename) ;
    virtual bool StartAudioByFeeder    (int feeder,int channel) ;
    virtual bool AttachFeeder          (int feeder,int channel) ;

    virtual bool LoadClipToStream      (int stream,SUID uuid,QPoint corner) ;
    virtual bool AttachDisplay         (int stream,int display) ;

    virtual void Queue                 (int Type,QPoint pos,const UUIDs & Uuids) ;

    virtual bool CreateStream          (int stream,SUID uuid,QString filename) ;
    virtual bool StartStream           (int stream) ;

  protected slots:

    virtual void PaperMoving           (QString name,QPointF pos,QPointF paper,QPointF scene);

    virtual void ItemModified          (VcfItem * item) ;

    virtual bool Menu                  (QPoint pos);
    virtual void PaperMenu             (VcfPaper     * paper    ,QPointF pos ) ;
    virtual void CanvasMenu            (VcfCanvas    * canvas   ,QPointF pos ) ;
    virtual void ShapeName             (VcfCanvas    * canvas   ,QString name) ;
    virtual void PictureMenu           (VcfPicture   * picture  ,QPointF pos ) ;
    virtual void ContourMenu           (VcfContours  * contour  ,QPointF pos ) ;
    virtual void ParagraphMenu         (VcfParagraph * paragraph,QPointF pos ) ;

    virtual void DeletePicture         (VcfPicture   * picture) ;
    virtual void StorePicture          (VcfPicture   * picture) ;
    virtual void PictureProcess        (VcfPicture   * picture) ;
    virtual void PictureFaces          (VcfPicture   * picture) ;
    virtual void PictureSelections     (VcfPicture   * picture) ;
    virtual void Selection             (VcfItem      * item,QRectF Screen) ;
    virtual void Canvas                (VcfItem      * item,QRectF Screen) ;
    virtual void StoreContours         (VcfCanvas    * canvas,VcfContours * contour) ;

  private slots:

  signals:

    void ShutdownAudio                 (int Channel) ;
    void Store                         (int method,SUID group,SUID uuid) ;

};

}

QT_END_NAMESPACE

#endif
