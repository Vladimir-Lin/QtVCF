#include <qtvcf.h>

N::VcfFlags:: VcfFlags     ( VcfHeadParaments  )
            : VcfInterface ( parent , item , p )
            , Item         ( NULL              )
            , Flags        ( NULL              )
{
}

N::VcfFlags::~VcfFlags(void)
{
}

void N::VcfFlags::AddItem(QString name,GraphicsItemFlag flag)
{
  GraphicsItemFlags F = Item->flags()                  ;
  bool              m = IsMask(F,flag)                 ;
  QIcon             i                                  ;
  if (m) i = QIcon         ( ":/images/yes.png"      ) ;
    else i = QIcon         ( ":/images/ready.png"    ) ;
  NewTreeWidgetItem        ( IT                      ) ;
  IT -> setData            ( 0 , Qt::UserRole , flag ) ;
  IT -> setText            ( 0 , name                ) ;
  IT -> setIcon            ( 0 , i                   ) ;
  Flags -> addTopLevelItem ( IT                      ) ;
}

void N::VcfFlags::List(void)
{
  Flags -> setColumnCount          ( 1           ) ;
  Flags -> setHeaderLabel          ( tr("Flag")  ) ;
  Flags -> setRootIsDecorated      ( false       ) ;
  Flags -> setAlternatingRowColors ( true        ) ;
  Flags -> clear                   (             ) ;
  plan  -> setFont                 ( Flags       ) ;
  #define ADDITEM(name) AddItem(#name,name)
  ADDITEM ( ItemIsMovable                        ) ;
  ADDITEM ( ItemIsSelectable                     ) ;
  ADDITEM ( ItemIsFocusable                      ) ;
  ADDITEM ( ItemClipsToShape                     ) ;
  ADDITEM ( ItemClipsChildrenToShape             ) ;
  ADDITEM ( ItemIgnoresTransformations           ) ;
  ADDITEM ( ItemIgnoresParentOpacity             ) ;
  ADDITEM ( ItemDoesntPropagateOpacityToChildren ) ;
  ADDITEM ( ItemStacksBehindParent               ) ;
  ADDITEM ( ItemUsesExtendedStyleOption          ) ;
  ADDITEM ( ItemHasNoContents                    ) ;
  ADDITEM ( ItemSendsGeometryChanges             ) ;
  ADDITEM ( ItemAcceptsInputMethod               ) ;
  ADDITEM ( ItemNegativeZStacksBehindParent      ) ;
  ADDITEM ( ItemIsPanel                          ) ;
  ADDITEM ( ItemSendsScenePositionChanges        ) ;
  #undef ADDITEM
}

bool N::VcfFlags::Initialize(QGraphicsItem * item,QRectF rect)
{
  QRect   RF( 0, 0,rect.width()   ,rect.height()   )              ;
  QRect   RC( 4,32,rect.width()- 8,rect.height()-36)              ;
  QRect   RH( 4, 4,rect.width()- 8,              24)              ;
  QPoint  CS(rect.left(),rect.top())                              ;
  QPointF CC                                                      ;
  CC = GraphicsView() -> mapToScene ( CS )                        ;
  CC = Options        -> Standard   ( CC )                        ;
  /////////////////////////////////////////////////////////////////
  Item       = item                                               ;
  Flags      = new QTreeWidget()                                  ;
  showHeader = true                                               ;
  HeaderRect = RH                                                 ;
  Title      = tr("QGraphicsItem flags")                          ;
  /////////////////////////////////////////////////////////////////
  setZValue ( 1.00f                 )                             ;
  setWidget ( Flags  , CC , RF , RC )                             ;
  /////////////////////////////////////////////////////////////////
  List ( )                                                        ;
  /////////////////////////////////////////////////////////////////
  connect(Flags,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int))   ,
          this ,SLOT  (doubleClicked    (QTreeWidgetItem*,int)) ) ;
  return true                                                     ;
}

void N::VcfFlags::doubleClicked(QTreeWidgetItem * item,int)
{
  GraphicsItemFlags F = Item->flags()                      ;
  int               f = item->data(0,Qt::UserRole).toInt() ;
  bool              m = !(IsMask(F,f))                     ;
  QIcon             i                                      ;
  if (m) i = QIcon         ( ":/images/yes.png"          ) ;
    else i = QIcon         ( ":/images/ready.png"        ) ;
  item -> setIcon          ( 0 , i                       ) ;
  Item -> setFlag          ( (GraphicsItemFlag)f , m     ) ;
}
