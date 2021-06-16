#include <qtvcf.h>

#define FONTID 4

N::VcfTasks:: VcfTasks    ( VcfHeadParaments  )
            : VcfBlocks   ( parent , item , p )
            , TaskManager ( p                 )
{
  Configure ( ) ;
}

N::VcfTasks::~VcfTasks(void)
{
}

void N::VcfTasks::Configure(void)
{
  Transform . reset (                                      ) ;
  ////////////////////////////////////////////////////////////
  Editable           = true                                  ;
  Borders [ Left   ] = 0.20                                  ;
  Borders [ Top    ] = 0.20                                  ;
  Borders [ Right  ] = 0.20                                  ;
  Borders [ Bottom ] = 0.20                                  ;
  ////////////////////////////////////////////////////////////
  setFlag            ( ItemIsMovable            , false    ) ;
  setFlag            ( ItemIsSelectable         , true     ) ;
  setFlag            ( ItemIsFocusable          , true     ) ;
  setFlag            ( ItemClipsToShape         , false    ) ;
  setFlag            ( ItemClipsChildrenToShape , true     ) ;
  ////////////////////////////////////////////////////////////
  Painter . addMap   ( "Default" , 0                       ) ;
  Painter . addPen   (      0 , QColor ( 192 , 192 , 192 ) ) ;
  Painter . addPen   ( FONTID , QColor (   0 ,   0 ,   0 ) ) ;
  Painter . addBrush (      0 , QColor ( 240 , 240 , 240 ) ) ;
  Painter . addBrush ( FONTID , QColor (   0 ,   0 ,   0 ) ) ;
}

void N::VcfTasks::Paint(QPainter * painter,QRectF Region,bool clip,bool color)
{
  VcfNode::Paint(painter,Region,clip,color) ;
}

void N::VcfTasks::Initialize(SUID uu)
{
  uuid     = uu                         ;
  TaskPlan = plan                       ;
  Options -> LineSpace = 0.1            ;
  ///////////////////////////////////////
  GraphicsManager GM( plan )            ;
  EnterSQL ( SC , plan->sql )           ;
    Painter . fonts [FONTID]            =
    GM . GetFont ( SC , "Default" )     ;
    Load   ( SC , uu        )           ;
  LeaveSQL ( SC , plan->sql )           ;
  ///////////////////////////////////////
  UUIDs Uuids   = TopLevels()           ;
  double ls     = Options->LineSpace    ;
  double LS     = ls * 2                ;
  double dy     = 0                     ;
  double bottom = 0                     ;
  double right  = 0                     ;
  QPointF P(ls+Borders[Left]            ,
            ls+Borders[Top ]   )        ;
  foreach (uuid,Uuids)                  {
    dy = addTopLevel( uuid , P )        ;
    P  . setY       ( LS + dy  )        ;
  }                                     ;
  ///////////////////////////////////////
  for (int i=0;i<children.count();i++)  {
    VcfTask * task                      ;
    task = VcfCasting(VcfTask,children[i]) ;
    if (NotNull(task))                  {
      QRectF SR = task->ScreenRect      ;
      SR = mapFromItem(task,SR)
           .boundingRect ( )            ;
      if (SR.bottom()>bottom)           {
        bottom = SR . bottom ()         ;
      }                                 ;
      if (SR.right()>right)             {
        right  = SR . right  ()         ;
      }                                 ;
    }                                   ;
  }                                     ;
  ///////////////////////////////////////
  QRectF  RS = ScreenRect               ;
  QPointF BR(right,bottom)              ;
  BR = Options->Standard(BR)            ;
  RS = Options->Standard(RS)            ;
  QRectF R(0                            ,
           0                            ,
           BR . x () + ls               ,
           BR . y () + ls               +
           Borders [ Bottom ]         ) ;
  if (RS.bottom()>R.bottom())           {
    R.setBottom(RS.bottom ())           ;
  }                                     ;
  if (RS.right ()>R.right ())           {
    R.setRight (RS.right  ())           ;
  }                                     ;
  setRect ( R )                         ;
}

N::VcfTask * N::VcfTasks::CreateTask(VcfNode * Parent,QRectF Range,SUID uuid,int nodeType)
{
  VcfTask  * vcf   = new VcfTask(GraphicsView(),Parent) ;
  vcf -> plan      = plan                               ;
  vcf -> name      = Names[uuid]                        ;
  vcf -> Alignment = Qt::AlignLeft | Qt::AlignVCenter   ;
  vcf -> Painter   . fonts [ FONTID ]                   =
         Painter   . fonts [ FONTID ]                   ;
  vcf -> setOptions  ( *Options , true )                ;
  vcf -> setZValue   ( 0.50f           )                ;
  vcf -> setOpacity  ( 1.00f           )                ;
  vcf -> setRange    ( Range           )                ;
  vcf -> setUuid     ( uuid , nodeType )                ;
  vcf -> Arrange     (                 )                ;
  vcf -> show        (                 )                ;
  connectNode        ( vcf             )                ;
  connect(vcf ,SIGNAL(Menu    (VcfNode*,QPointF))       ,
          this,SLOT  (TaskMenu(VcfNode*,QPointF))     ) ;
  return vcf                                            ;
}

void N::VcfTasks::MountLinker(VcfLinker * linker,VcfTask * first,VcfTask * second)
{
  linker -> EnableArrow   ( 0      , false          ) ;
  linker -> EnableArrow   ( 1      , true           ) ;
  linker -> setFirst      ( first                   ) ;
  linker -> setSecond     ( second                  ) ;
  linker -> setPenColor   ( 1 , QColor(255,128,  0) ) ;
  linker -> setBrushColor ( 1 , QColor(255,192,128) ) ;
  linker -> Prepare       ( false  , false          ) ;
  connectLinker           ( linker                  ) ;
  Mount                   ( linker , first , second ) ;
}

void N::VcfTasks::MeltLinker(VcfLinker * linker,QPointF pz,QPointF pe)
{
  VcfNode * first  = linker->Linking[0] ;
  VcfNode * second = linker->Linking[1] ;
  if (IsNull(first )) return            ;
  if (IsNull(second)) return            ;
  pz = Options -> position ( pz )       ;
  pe = Options -> position ( pe )       ;
  pz = mapToItem ( first  , pz  )       ;
  pe = mapToItem ( second , pe  )       ;
  pz = Options -> Standard ( pz )       ;
  pe = Options -> Standard ( pe )       ;
  first  -> AttemptMelt ( linker , pz ) ;
  second -> AttemptMelt ( linker , pe ) ;
}

double N::VcfTasks::addTopLevel(SUID uu,QPointF position)
{
  int    nType = Behaviors [ uu ]           ;
  QRectF R ( position , QSizeF(6,0.72) )    ;
  VcfTask * task = CreateTask               (
                     this                   ,
                     R                      ,
                     uu                     ,
                     nType                ) ;
  ///////////////////////////////////////////
  // Questionable, should be
  // Related << task
  // and
  // Nodes is for linking
  addChildren         ( task            )   ;
  ///////////////////////////////////////////
  emit Append         ( task , this     )   ;
  if (nodeType==Tasks::Framework)           {
    addFramework ( task )                   ;
  }                                         ;
  task -> Arrange     (                 )   ;
  if (isSuccessor     ( uu              ) ) {
    addSuccessor      ( NULL , task     )   ;
  }                                         ;
  task -> Arrange     (                 )   ;
  ///////////////////////////////////////////
  double dy = 0                             ;
  for (int i=0;i<children.count();i++)      {
    VcfTask * node                          ;
    node = VcfCasting(VcfTask,children[i])  ;
    if (NotNull(node))                      {
      QRectF    nr   = node->ScreenRect     ;
      nr = node -> mapToItem    ( this,nr   )
                 . boundingRect (         ) ;
      nr = Options->Standard    ( nr      ) ;
      if (nr.bottom()>dy) dy = nr.bottom()  ;
    }                                       ;
  }                                         ;
  ///////////////////////////////////////////
  return dy                                 ;
}

void N::VcfTasks::addFramework(VcfTask * task)
{
  SUID  uu     = task->node  ;
  UUIDs Frames = Frame(uu)   ;
  SUID  fuid                 ;
  foreach (fuid,Frames)      {
    joinFramework(task,fuid) ;
  }                          ;
}

void N::VcfTasks::joinFramework(VcfTask * task,SUID uuid)
{
  int     nodeType = Behaviors [ uuid ]               ;
  QPointF position = task -> Baseline()               ;
  QRectF  R ( position , QSizeF(6,0.72) )             ;
  VcfTask * vcf = CreateTask                          (
                     task                             ,
                     R                                ,
                     uuid                             ,
                     nodeType                       ) ;
  addChildren       ( vcf )                           ;
  task->addChildren ( vcf )                           ;
  emit Append       ( vcf , task )                    ;
  if (nodeType==Tasks::Framework)           {
    addFramework ( vcf )                              ;
  }                                                   ;
  vcf -> Arrange  (            )                      ;
  if (isSuccessor ( uuid       ) )                    {
    addSuccessor  ( task , vcf )                      ;
  }                                                   ;
  if (Heirs.contains(uuid))                           {
      UUIDs Huids = Heirs.values(uuid)                ;
      SUID  suid ;
      foreach (suid,Huids) {
        PlanDebug(15,tr("%1 has heir %2").arg(vcf->name).arg(Names[suid]));
      };
  }                                                   ;
  vcf -> Arrange  (            )                      ;
}

void N::VcfTasks::addSuccessor(VcfTask * task,VcfTask * predecessor)
{
  SUID      suid  = predecessor->uuid                  ;
  SUID      uuid  = Successors[suid]                   ;
  QRectF    prect = predecessor->ScreenRect            ;
  VcfTask * pTask = task                               ;
  if (IsNull(pTask)) pTask = (VcfTask *)this           ;
  prect = predecessor -> mapToItem    ( pTask,prect    )
                       . boundingRect (              ) ;
  prect = Options -> Standard ( prect )                ;
  //////////////////////////////////////////////////////
  int     nodeType = Behaviors [ uuid ]                ;
  QPointF position = prect.topRight() + QPointF(1.5,0) ;
  QRectF  R ( position , QSizeF(6,0.72) )              ;
  VcfTask * vcf = CreateTask                           (
                     pTask                             ,
                     R                                 ,
                     uuid                              ,
                     nodeType                        ) ;
  addChildren ( vcf )                                  ;
  if (NotNull(task)) task->addChildren(vcf)            ;
  emit Append        ( vcf , pTask     )               ;
  if (nodeType==Tasks::Framework)            {
    addFramework ( vcf )                               ;
  }                                                    ;
  vcf -> Arrange  (            )                       ;
  //////////////////////////////////////////////////////
  // calculate both side
  QRectF prt = predecessor -> ScreenRect               ;
  QRectF vrt = vcf         -> ScreenRect               ;
  prt = predecessor -> mapToItem    ( pTask,prt    )
                     . boundingRect (              )   ;
  vrt = vcf         -> mapToItem    ( pTask,vrt    )
                     . boundingRect (              )   ;
  prt = Options -> Standard ( prt )                    ;
  vrt = Options -> Standard ( vrt )                    ;
  QPointF pct = prt.center()                           ;
  QPointF vct = vrt.center()                           ;
  QPointF ps         ( prt . right () , pct . y ()  )  ;
  QPointF pe         ( vrt . left  () , vct . y ()  )  ;
  VcfLinker * linker                                   ;
  linker  = CreateLinker  ( pTask  , ps , pe        )  ;
  MountLinker          ( linker , predecessor , vcf )  ;
  MeltLinker           ( linker , ps          , pe  )  ;
  emit Append             ( linker , pTask          )  ;
  //////////////////////////////////////////////////////
  if (isSuccessor ( uuid       ) )                     {
    addSuccessor  ( task , vcf )                       ;
  }                                                    ;
  if (Heirs.contains(uuid))                            {
      UUIDs Huids = Heirs.values(uuid)                 ;
      SUID  suid ;
      foreach (suid,Huids) {
        PlanDebug(15,tr("%1 has heir %2").arg(vcf->name).arg(Names[suid]));
      };
  }                                                    ;
  vcf -> Arrange  (            )                       ;
}

void N::VcfTasks::Hovering(QPointF pos)
{
}

void N::VcfTasks::AcceptDissolve(VcfItem * item)
{
}

void N::VcfTasks::AcceptMoving(VcfItem * item)
{
  VcfTask * task = VcfCasting ( VcfTask , item )        ;
  if (IsNull(task)) return                              ;
  if (task->Connectors()<=0) return                     ;
  for (int i=0;i<task->Connectors();i++)                {
    Relation * related = task->Connector(i)             ;
    nIfSafe(related)                                    {
      VcfLinker * linker = (VcfLinker *)related         ;
      linker = VcfCasting(VcfLinker,linker)             ;
      if (NotNull(linker))                              {
        bool changed = false                            ;
        if (linker->isFirst(task))                      {
          QPointF p = linker->Melts[0]                  ;
          p = task->PaperPosition(this,p)               ;
          linker -> setFirst ( p )                      ;
          changed = true                                ;
        }                                               ;
        if (linker->isEnd  (task))                      {
          QPointF p = linker->Melts[1]                  ;
          p = task->PaperPosition(this,p)               ;
          linker -> setEnd   ( p )                      ;
          changed = true                                ;
        }                                               ;
        if (changed)                                    {
          linker -> Prepare ( )                         ;
          if (linker->hasFirst())                       {
            VcfNode * first  = linker->Linking[0]       ;
            QPainterPath p = first ->Painter.pathes[1]  ;
            p = first ->mapToItem(linker,p)             ;
            linker->ClipPath(p)                         ;
          }                                             ;
          if (linker->hasSecond())                      {
            VcfNode * second = linker->Linking[1]       ;
            QPainterPath p = second->Painter.pathes[1]  ;
            p = second->mapToItem(linker,p)             ;
            linker->ClipPath(p)                         ;
          }                                             ;
          linker -> update  ( )                         ;
        }                                               ;
      }                                                 ;
    }                                                   ;
  }                                                     ;
  update ( )                                            ;
}

bool N::VcfTasks::Menu(QPointF pos)
{
  nScopedMenu(mm,GraphicsView()) ;
  mm.add(101,tr("Add"));
  mm.exec();
  return true                    ;
}

void N::VcfTasks::TaskMenu(VcfNode * node,QPointF pos)
{
  nScopedMenu(mm,GraphicsView()) ;
  mm.add(101,tr("Task menu"));
  mm.exec();
}

#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void nTasks::list(QTreeWidget * tree)
{
  list(tree,Tasks);
}

void nTasks::list(QTreeWidget * tree,UUIDs & Uuids)
{
  SUID tuid ;
  tree->clear();
  foreach (tuid,Uuids) {
    QTreeWidgetItem * item     = NULL ;
    int               behavior = Tasks::Undecided ;
    if (Behaviors.contains(tuid)) behavior = Behaviors[tuid];
    item = new QTreeWidgetItem();
    item->setData(0,Qt::UserRole,tuid);
    item->setText(0,Names[tuid]);
    setIcon(item,behavior);
    tree->addTopLevelItem(item);
  };
}

void nTasks::list(QComboBox * combo,UUIDs & Uuids,SUID index)
{
  SUID uuid;
  int  idx = -1;
  combo->clear();
  foreach (uuid,Uuids) {
    if (uuid==index) idx = combo->count();
    combo->addItem(Names[uuid],uuid);
  };
  if (idx>=0) combo->setCurrentIndex(idx);
}

void nTasks::setIcon(QTreeWidgetItem * item,int behavior)
{
  switch (behavior) {
    case Tasks::Undecided : item->setIcon(0,QIcon(":/images/undecided.png" )); break;
    case Tasks::Standalone: item->setIcon(0,QIcon(":/images/standalone.png")); break;
    case Tasks::Framework : item->setIcon(0,QIcon(":/images/framework.png" )); break;
    case Tasks::Decision  : item->setIcon(0,QIcon(":/images/decision.png"  )); break;
    case Tasks::Obsolete  : item->setIcon(0,QIcon(":/images/obsolete.png"  )); break;
  };
}

void nTasks::listFramework(QComboBox * combo,SUID uuid)
{
  UUIDs af = Frameworks();
  UUIDs ef = Frameworks(uuid);
  SUID  iu = 0;
  SUID  eu = 0;
  if (Parents.contains(uuid)) {
    eu = Parents[uuid];
    if (!af.contains(eu)) af.prepend(eu);
  };
  ef << uuid ;
  foreach (iu,ef) {
    int index = af.indexOf(iu);
    if (index>=0) af.takeAt(index);
  };
  list(combo,af,eu);
}

void nTasks::listFramework(QTreeWidget * tree,SUID uuid)
{
  UUIDs Uuids ;
  SUID  tuid  ;
  foreach (tuid,Tasks) {
    if (Parents.contains(tuid)) {
      if (Parents[tuid]==uuid) Uuids << tuid ;
    };
  };
  list(tree,Uuids);
}

bool nTasks::joinFrameWork(nSystemPlan * Plan,QSqlQuery & q,SUID uuid,SUID fuid,bool root)
{
  nSqlSettings Sql = Plan->Sql ;
  QString      Q               ;
  detachFrameWork(Plan,q,uuid) ;
  if (fuid<=0) return false    ;
  Parents [uuid] = fuid ;
  Roots   [uuid] = root ;
  if (root) {
    Q = Sql.Update(Plan->Tables[Tables::TaskRelations]   ) +
            " relation = " + QString::number(Tasks::Child) +
        " where second = " + QString::number(fuid                  ) +
        " and relation = " + QString::number(Tasks::Root ) + ";";
    q.exec(Q);
  };
  Q = Sql.ReplaceInto(
        "first,second,relation",
        Plan->Tables[Tables::TaskRelations],
        QString("%1,%2,%3")
              .arg(uuid)
              .arg(fuid)
              .arg(root ? Tasks::Root : Tasks::Child)
      ) + ";" ;
  q.exec(Q);
  return true;
}

bool nTasks::detachFrameWork(nSystemPlan * Plan,QSqlQuery & q,SUID uuid)
{
  nSqlSettings Sql = Plan->Sql                                      ;
  QString      Q                                                    ;
  SUID         fuid = 0                                             ;
  if (Parents.contains(uuid)) fuid = Parents[uuid]                  ;
  Roots[uuid] = false                                               ;
  Parents.remove(uuid)                                              ;
  if (fuid<=0) return false                                         ;
  Q = Sql.DeleteFrom(Plan->Tables[Tables::TaskRelations]) +
      " where first = " + QString::number(uuid)                     +
       " and second = " + QString::number(fuid)               + ";" ;
  q.exec(Q)                                                         ;
  return true                                                       ;
}

void nTasks::listPredecessors(QComboBox * combo,SUID uuid)
{
  UUIDs af = Predecessor();
  UUIDs ef = Predecessor(uuid);
  SUID  iu = 0;
  SUID  eu = 0;
  if (Predecessors.contains(uuid)) {
    eu = Predecessors[uuid];
    if (!af.contains(eu)) af.prepend(eu);
  };
  ef << uuid ;
  foreach (iu,ef) {
    int index = af.indexOf(iu);
    if (index>=0) af.takeAt(index);
  };
  list(combo,af,eu);
}

bool nTasks::updatePredecessor(nSystemPlan * Plan,QSqlQuery & q,SUID uuid,SUID tuid,SUID puid)
{
  nSqlSettings Sql = Plan->Sql ;
  QString      Q ;
  if (Predecessors.contains(uuid)) Predecessors.remove(uuid) ;
  if (Successors  .contains(puid)) Successors  .remove(puid) ;
  if (Behaviors[puid]==Tasks::Decision) {
    UUIDs ss = Heirs.values(puid);
    if (ss.contains(uuid)) Heirs.remove(puid,uuid);
  };
  if (puid>0) {
    Q = Sql.DeleteFrom(Plan->Tables[Tables::TaskRelations]) +
        " where first = " + QString::number(uuid) +
         " and second = " + QString::number(puid) + ";";
    q.exec(Q);
    Q = Sql.DeleteFrom(Plan->Tables[Tables::TaskRelations]) +
        " where first = " + QString::number(puid) +
         " and second = " + QString::number(uuid) + ";";
    q.exec(Q);
  };
  if (tuid>0) {
    if (Behaviors[tuid]==Tasks::Decision) {
      UUIDs ff = Heirs.values(tuid);
      if (!ff.contains(uuid)) Heirs.insert(tuid,uuid);
      Predecessors[uuid] = tuid ;
    } else {
      Predecessors[uuid] = tuid ;
      Successors  [tuid] = uuid ;
    };
    Q = Sql.ReplaceInto(
                "first,second,relation",
                Plan->Tables[Tables::TaskRelations],
                QString("%1,%2,%3")
                .arg(uuid)
                .arg(tuid)
                .arg(Tasks::Prerequiste)
        ) + ";";
    q.exec(Q);
    Q = Sql.ReplaceInto(
                "first,second,relation",
                Plan->Tables[Tables::TaskRelations],
                QString("%1,%2,%3")
                .arg(tuid)
                .arg(uuid)
                .arg(Tasks::Heir)
        ) + ";";
    q.exec(Q);
  };
  return true;
}

void nTasks::listSuccessors(QComboBox * combo,SUID uuid)
{
  UUIDs af = Successor();
  UUIDs ef = Successor(uuid);
  SUID  iu = 0;
  SUID  eu = 0;
  if (Successors.contains(uuid)) {
    eu = Successors[uuid];
    if (!af.contains(eu)) af.prepend(eu);
  };
  ef << uuid ;
  foreach (iu,ef) {
    int index = af.indexOf(iu);
    if (index>=0) af.takeAt(index);
  };
  list(combo,af,eu);
}

bool nTasks::updateSuccessor(nSystemPlan * Plan,QSqlQuery & q,SUID uuid,SUID tuid,SUID puid)
{
  nSqlSettings Sql = Plan->Sql ;
  QString      Q ;
  if (Predecessors.contains(puid)) Predecessors.remove(puid) ;
  if (Successors  .contains(uuid)) Successors  .remove(uuid) ;
  if (Behaviors[puid]==Tasks::Decision) {
    UUIDs ff = Entries.values(puid);
    if (ff.contains(uuid)) Entries.remove(puid,uuid);
  };
  if (puid>0) {
    Q = Sql.DeleteFrom(Plan->Tables[Tables::TaskRelations]) +
        " where first = " + QString::number(uuid) +
         " and second = " + QString::number(puid) + ";";
    q.exec(Q);
    Q = Sql.DeleteFrom(Plan->Tables[Tables::TaskRelations]) +
        " where first = " + QString::number(puid) +
         " and second = " + QString::number(uuid) + ";";
    q.exec(Q);
  };
  if (tuid>0) {
    if (Behaviors[tuid]==Tasks::Decision) {
      UUIDs ss = Entries.values(tuid);
      if (!ss.contains(uuid)) Entries.insert(tuid,uuid);
      Successors  [uuid] = tuid ;
    } else {
      Predecessors[tuid] = uuid ;
      Successors  [uuid] = tuid ;
    };
    Q = Sql.ReplaceInto(
                "first,second,relation",
                Plan->Tables[Tables::TaskRelations],
                QString("%1,%2,%3")
                .arg(tuid)
                .arg(uuid)
                .arg(Tasks::Prerequiste)
        ) + ";";
    q.exec(Q);
    Q = Sql.ReplaceInto(
                "first,second,relation",
                Plan->Tables[Tables::TaskRelations],
                QString("%1,%2,%3")
                .arg(uuid)
                .arg(tuid)
                .arg(Tasks::Heir)
        ) + ";";
    q.exec(Q);
  };
  return true;
}

UUIDs nTasks::Candidates(void)
{
  UUIDs  Uuids ;
  SUID   uuid  ;
  foreach (uuid,Tasks) {
    switch (Behaviors[uuid]) {
      case Tasks::Standalone :
        if (!Predecessors.contains(uuid) ||
            !Successors  .contains(uuid)  ) Uuids << uuid ;
      break;
      case Tasks::Framework  :
        if (!Predecessors.contains(uuid) ||
            !Successors  .contains(uuid)  ) Uuids << uuid ;
      break;
      case Tasks::Decision   :
        Uuids << uuid ;
      break;
    };
  };
  return Uuids ;
}

UUIDs nTasks::Candidates(SUID uuid)
{
  UUIDs  Uuids   ;
  SUID   puid    ;
  bool   notjoin ;
  foreach (puid,Tasks) {
    switch (Behaviors[puid]) {
      case Tasks::Standalone :
        notjoin = Predecessors.contains(puid) && Successors.contains(puid) ;
        if (notjoin) Uuids << puid ;
      break;
      case Tasks::Framework  :
        notjoin = Predecessors.contains(puid) && Successors.contains(puid) ;
        if (notjoin) Uuids << puid ;
      break;
      case Tasks::Decision   :
        if (uuid==puid) Uuids << puid ;
      break;
    };
  };
  return Uuids ;
}

void nTasks::listCandidates(QComboBox * combo,SUID uuid)
{
  UUIDs af = Candidates();
  UUIDs ef = Candidates(uuid);
  SUID  iu = 0;
  foreach (iu,ef) {
    int index = af.indexOf(iu);
    if (index>=0) af.takeAt(index);
  };
  list(combo,af,uuid);
}

void nTasks::decisionPredecessor(QTreeWidget * tree,SUID uuid)
{
  UUIDs Uuids = Entries.values(uuid);
  list(tree,Uuids);
}

void nTasks::decisionSuccessor(QTreeWidget * tree,SUID uuid)
{
  UUIDs Uuids = Heirs.values(uuid);
  list(tree,Uuids);
}

void nTasks::addPredecessor(nSystemPlan * Plan,QSqlQuery & q,SUID uuid,SUID tuid)
{
  nSqlSettings Sql = Plan->Sql ;
  QString      Q ;
  if (Behaviors[tuid]==Tasks::Decision) {
    Heirs   . insert (tuid,uuid) ;
    Entries . insert (uuid,tuid) ;
  } else {
    Entries . insert (uuid,tuid) ;
    Successors[tuid] = uuid      ;
  };
  Q = Sql.ReplaceInto(
              "first,second,relation",
              Plan->Tables[Tables::TaskRelations],
              QString("%1,%2,%3")
              .arg(uuid)
              .arg(tuid)
              .arg(Tasks::Prerequiste)
      ) + ";";
  q.exec(Q);
  Q = Sql.ReplaceInto(
              "first,second,relation",
              Plan->Tables[Tables::TaskRelations],
              QString("%1,%2,%3")
              .arg(tuid)
              .arg(uuid)
              .arg(Tasks::Heir)
      ) + ";";
  q.exec(Q);
}

void nTasks::addHeir(nSystemPlan * Plan,QSqlQuery & q,SUID uuid,SUID tuid)
{
  nSqlSettings Sql = Plan->Sql ;
  QString      Q ;
  if (Behaviors[tuid]==Tasks::Decision) {
    Heirs   . insert (uuid,tuid) ;
    Entries . insert (tuid,uuid) ;
  } else {
    Heirs   . insert (uuid,tuid) ;
    Predecessors[tuid] = uuid ;
  };
  Q = Sql.ReplaceInto(
              "first,second,relation",
              Plan->Tables[Tables::TaskRelations],
              QString("%1,%2,%3")
              .arg(tuid)
              .arg(uuid)
              .arg(Tasks::Prerequiste)
      ) + ";";
  q.exec(Q);
  Q = Sql.ReplaceInto(
              "first,second,relation",
              Plan->Tables[Tables::TaskRelations],
              QString("%1,%2,%3")
              .arg(uuid)
              .arg(tuid)
              .arg(Tasks::Heir)
      ) + ";";
  q.exec(Q);
}

void nTasks::listHierarchies(QTreeWidget * tree,SUID uuid)
{
  UUIDs Uuids ;
  SUID  tuid  ;
  foreach (tuid,Tasks) {
    if (tuid==uuid) {
      //
    } else {
      if (!Parents.contains(tuid)) Uuids << tuid ;
    };
  };
  list(tree,Uuids);
}

///////////////////////////////////////////////////////////////////////////////

TaskRectangle:: TaskRectangle     (QObject * parent,QGraphicsItem * item)
             :  QObject           (          parent                     )
             ,  QGraphicsRectItem (                                 item)
{
}

TaskRectangle::~TaskRectangle(void)
{
}

QRectF TaskRectangle::boundingRect (void) const
{
  return rect;
}

bool TaskRectangle::contains(const QPointF & point) const
{
  return rect.contains(point);
}

void TaskRectangle::paint(QPainter * painter,const QStyleOptionGraphicsItem *, QWidget *)
{
  Paint ( painter ) ;
}

void TaskRectangle::Paint(QPainter * p)
{
  QPen pen = QColor(0,0,0);
  if (isSelected()) pen.setStyle(Qt::DotLine);
  p->setPen(pen);
  switch (Behavior) {
    case Tasks::Standalone: PaintStandalone(p); break;
    case Tasks::Framework : PaintFramework (p); break;
    case Tasks::Decision  : PaintOctagon   (p); break;
  };
  p->setFont(Texts->font(FontId));
  p->drawText(text,Qt::AlignHCenter|Qt::AlignVCenter,Name);
}

void TaskRectangle::PaintOctagon(QPainter * p)
{
  QPointF b = Plan->toPaper(QPointF(0.2,0.2),Plan->DPI);
  QPolygonF P                                          ;
  P << QPointF(rect.left ()+b.x(),rect.top   ()      ) ;
  P << QPointF(rect.right()-b.x(),rect.top   ()      ) ;
  P << QPointF(rect.right()      ,rect.top   ()+b.y()) ;
  P << QPointF(rect.right()      ,rect.bottom()-b.y()) ;
  P << QPointF(rect.right()-b.x(),rect.bottom()      ) ;
  P << QPointF(rect.left ()+b.x(),rect.bottom()      ) ;
  P << QPointF(rect.left ()      ,rect.bottom()-b.y()) ;
  P << QPointF(rect.left ()      ,rect.top   ()+b.y()) ;
  p->drawPolygon(P)                                    ;
}

void TaskRectangle::PaintFramework(QPainter * p)
{
  QPointF b = Plan->toPaper(QPointF(0.1,0.1),Plan->DPI);
  p->drawRect(rect);
  p->drawLine(rect.left()+b.x(),rect.top()+b.y(),rect.right()-b.x(),rect.top()+b.y());
}

void TaskRectangle::PaintStandalone(QPainter * p)
{
  p->drawRect(rect);
}

QRectF TaskRectangle::evaluate(void)
{
  QFont f = Texts->font(FontId);
  QFontMetricsF m(f);
  QRectF  t = m.boundingRect(Name);
  QPointF b = Plan->toPaper(QPointF(0.6,0.6),Plan->DPI);
  return QRectF(0,0,t.width()+b.x(),t.height()+b.y());
}

void TaskRectangle::setPos(QPointF start)
{
  rect.setLeft(start.x());
  rect.setTop(start.y());
}

void TaskRectangle::setSize(QSizeF s)
{
  QPointF b = Plan->toPaper(QPointF(0.25,0.25),Plan->DPI);
  rect.setWidth(s.width());
  rect.setHeight(s.height());
  text.setLeft(rect.left()+b.x());
  text.setTop(rect.top()+b.y());
  text.setRight(rect.right()-b.x());
  text.setBottom(rect.bottom()-b.y());
  setRect(rect);
}

void TaskRectangle::moveSize(QSizeF s)
{
  QRectF  T = text ;
  QPointF D(T.left()-rect.left(),T.top()-rect.top());
  setSize(s);
  text.setLeft(rect.left()+D.x());
  text.setTop(rect.top()+D.y());
  text.setRight(rect.right()-D.x());
  text.setHeight(T.height());
}

///////////////////////////////////////////////////////////////////////////////

TaskEditor:: TaskEditor    (QWidget *    parent)
          :  QGraphicsView (             parent)
          ,  Plan          (NULL               )
          ,  Tasks         (NULL               )
          ,  Texts         (NULL               )
          ,  Project       (0                  )
{
  setScene(&Geometry.Scene) ;
}

TaskEditor::~TaskEditor (void)
{
}

void TaskEditor::contextMenuEvent(QContextMenuEvent * event)
{
  QGraphicsView::contextMenuEvent(event);
}

void TaskEditor::mouseDoubleClickEvent(QMouseEvent * event)
{
  QGraphicsView::mouseDoubleClickEvent(event);
}

void TaskEditor::mouseMoveEvent(QMouseEvent * event)
{
  QGraphicsView::mouseMoveEvent(event);
}

void TaskEditor::mousePressEvent(QMouseEvent * event)
{
  QGraphicsView::mousePressEvent(event);
}

void TaskEditor::mouseReleaseEvent(QMouseEvent * event)
{
  QGraphicsView::mouseReleaseEvent(event);
}

void TaskEditor::resizeEvent(QResizeEvent * event)
{
  QGraphicsView::resizeEvent(event)       ;
}

void TaskEditor::wheelEvent(QWheelEvent * event)
{
  QGraphicsView::wheelEvent(event);
}

void TaskEditor::Prepare(void)
{
  Ratios.setParent(this)      ;
  Ratios.Plan     = Plan      ;
  Ratios.Geometry = &Geometry ;
  Ratios.DPI      = Plan->DPI ;
  Ratios.Initialize(this)     ;
  Ratios.Transform()          ;
  ///////////////////////////////////////////////////////////////////////////
  setScene                     (&Geometry.Scene               ) ;
  setTransform                 (Geometry.Transform            ) ;
  setCacheMode                 (QGraphicsView::CacheBackground) ;
  setMouseTracking             (true                          ) ;
  setFocusPolicy               (Qt::WheelFocus                ) ;
  ///////////////////////////////////////////////////////////////////////////
  Ratios.Translate(this,QRectF(0,0,1,1));
  Plan->processEvents();
  Clear();
  Create();
  if (TaskRectangles.count()>0) Ratios.Translate(this,Rectangles());
}

void TaskEditor::Clear(void)
{
  UUIDs Uuids = TaskRectangles.keys();
  SUID  uuid ;
  if (Uuids.count()<=0) return;
  foreach (uuid,Uuids) TaskRectangles[uuid]->deleteLater();
  TaskRectangles.clear();
}

QRectF TaskEditor::Rectangles(void)
{
  UUIDs Uuids = TaskRectangles.keys();
  QRectF TR = Rectangles(Uuids);
  QPointF U = Plan->toPaper(QPointF(0.30,0.30),Plan->DPI);
  return QRectF(0,0,TR.right()+U.x(),TR.bottom()+U.y());
}

QRectF TaskEditor::Rectangles(UUIDs & Uuids)
{
  QRectF R(0,0,0,0);
  if (Uuids.count()<=0) return R;
  R = TaskRectangles[Uuids[0]]->boundingRect();
  for (int i=1;i<Uuids.count();i++)
    R = R.united(TaskRectangles[Uuids[i]]->boundingRect());
  return R;
}

void TaskEditor::Create(void)
{
  UUIDs   t = Tasks->TopLevel();
  SUID    u;
  foreach (u,t) {
    TaskRectangle * r  = create(u) ;
    Create(r);
  };
}

TaskRectangle * TaskEditor::create(SUID uuid)
{
  QPointF b = Plan->toPaper(QPointF(1.0,0.25),Plan->DPI) ;
  TaskRectangle * r = new TaskRectangle(this,NULL)       ;
  ////////////////////////////////////////////////////////
  r->Plan     = Plan                                     ;
  r->Texts    = Texts                                    ;
  r->View     = this                                     ;
  r->DPI      = Plan->DPI                                ;
  r->Uuid     = uuid                                     ;
  r->FontId   = FontId                                   ;
  r->Name     = Tasks->Names    [uuid]                   ;
  r->Behavior = Tasks->Behaviors[uuid]                   ;
  r->Status   = Tasks->Status   [uuid]                   ;
  r->Flags    = Tasks->Flags    [uuid]                   ;
  ////////////////////////////////////////////////////////
  QRectF eb = r->evaluate ()                             ;
  QRectF rs = Rectangles  ()                             ;
  ////////////////////////////////////////////////////////
  r->setToolTip(r->Name)                                 ;
  r->setZValue(0.20f)                                    ;
  r->setPos (QPointF(b.x(),rs.bottom()+b.y()))           ;
  r->setSize(QSizeF(eb.width(),eb.height()))             ;
  r->setFlag(QGraphicsItem::ItemIsSelectable,true)       ;
  r->setFlag(QGraphicsItem::ItemIsFocusable ,true)       ;
  r->setFlag(QGraphicsItem::ItemIsMovable   ,true)       ;
  r->setAcceptHoverEvents(true)                          ;
  ////////////////////////////////////////////////////////
  TaskRectangles[uuid] = r                               ;
  Geometry.Scene.addItem(r)                              ;
  r->update()                                            ;
  return r                                               ;
}

UUIDs TaskEditor::Create(TaskRectangle * task)
{
  QRectF  R = task->boundingRect();
  QPointF U = Plan->toPaper(QPointF(0.30,1.0),Plan->DPI);
  QPointF b = Plan->toPaper(QPointF(1.0,0.25),Plan->DPI) ;
  QPointF S(R.left()+U.x(),R.bottom());
  UUIDs  Uuids ;
  UUIDs  Frame ;
  switch (task->Behavior) {
    case Tasks::Standalone :
      if (Tasks->Successors.contains(task->Uuid)) {
        TaskRectangle * r  = create(Tasks->Successors[task->Uuid]) ;
        Uuids << r->Uuid ;
        QRectF  w = r->boundingRect();
        QPointF s(R.right()+U.y(),R.top());
        QSizeF  g(w.width(),w.height());
        r->setPos(s);
        r->setSize(g);
        r->setParentItem(task);
        Uuids << Create(r);
      };
    break                            ;
    case Tasks::Framework  :
      Frame = Tasks->Frame(task->Uuid);
      if (Frame.count()>0) {
        UUIDs f;
        SUID  u;
        foreach (u,Frame) {
          TaskRectangle * r  = create(u) ;
          QRectF  w = r->boundingRect();
          QSizeF  g(w.width(),w.height());
          QPointF s;
          if (f.count()<=0) s = S; else {
            QRectF z = Rectangles(f);
            s.setX(S.x());
            s.setY(z.bottom()+b.y());
          };
          f << r->Uuid ;
          r->setPos(s);
          r->setSize(g);
          r->setParentItem(task);
          f << Create(r);
        };
        QRectF z = Rectangles(f);
        QRectF x;
        x.setLeft(R.left());
        x.setTop(R.top());
        x.setRight(z.right()+U.x());
        x.setBottom(z.bottom()+U.x());
        if (R.right()>x.right()) x.setRight(R.right());
        task->moveSize(QSizeF(x.width(),x.height()));
        Uuids << f;
      };
      if (Tasks->Successors.contains(task->Uuid)) {
        R = task->boundingRect();
        TaskRectangle * r  = create(Tasks->Successors[task->Uuid]) ;
        Uuids << r->Uuid ;
        QRectF  w = r->boundingRect();
        QPointF s(R.right()+U.y(),R.top());
        QSizeF  g(w.width(),w.height());
        r->setPos(s);
        r->setSize(g);
        r->setParentItem(task);
        Uuids << Create(r);
      };
    break                            ;
    case Tasks::Decision   :
    break                            ;
  };
  return Uuids ;
}

///////////////////////////////////////////////////////////////////////////////

TaskManager:: TaskManager    (QWidget *         parent)
           :  QStackedWidget (                  parent)
           ,  ui             (new Ui::TaskManager     )
           ,  Editor         (NULL                    )
           ,  NameEditor     (NULL                    )
           ,  Selector       (NULL                    )
           ,  ItemEditing    (NULL                    )
           ,  Plan           (NULL                    )
           ,  Tasks          (NULL                    )
           ,  Texts          (NULL                    )
           ,  Project        (0                       )
           ,  FontId         (Fonts::Keyword)
{
  ui->setupUi(this);
  Editor = new TaskEditor(this);
  addWidget(Editor);
}

TaskManager::~TaskManager (void)
{
  delete ui;
}

void TaskManager::resizeEvent(QResizeEvent * event)
{
  QStackedWidget::resizeEvent(event);
  Relocation();
}

void TaskManager::Relocation(void)
{
  QWidget * w = currentWidget();
  if (IsNull(w)) return ;
  QSize W(w->width(),w->height());
  ui->projectWidget->resize(W.width(),W.height()-ui->projectWidget->y());
  ui->taskWidget->resize(ui->taskWidget->width(),W.height()-ui->taskWidget->y());
  ui->taskProperty->resize(W.width()-ui->taskProperty->x(),W.height()-ui->taskWidget->y());
  W . setWidth  (W.width  () - ui->taskProperty->x()) ;
  W . setHeight (W.height () - ui->taskWidget  ->y()) ;
  int dx = ui->predecessorWidget->x() ;
  int dy = dx                         ;
  int dw = W.width() - ( dx * 3 )     ; dw /= 2;
  ui->predecessorWidget->resize(dw,W.height()-ui->predecessorWidget->y()-dy);
  ui->successorWidget->move(dw+(dx*2),ui->successorWidget->y());
  ui->successorWidget->resize(dw,W.height()-ui->predecessorWidget->y()-dy);
  dw = W.width() - (dx * 4) - ui->joinButton->width(); dw /= 2;
  ui->childWidget->resize(dw,W.height()-ui->childWidget->y()-dy);
  ui->standaloneWidget->move(W.width()-dx-dw,ui->childWidget->y());
  ui->standaloneWidget->resize(dw,W.height()-ui->childWidget->y()-dy);
  ui->joinButton->move(dw+(dx*2),ui->joinButton->y());
  ui->detachButton->move(dw+(dx*2),ui->detachButton->y());
}

void TaskManager::setFonts(void)
{
  setFont(Plan->Fonts[Fonts::Default]);
  ui->taskProperty     ->setFont(Plan->Fonts[Fonts::Default]);
  ui->projectWidget    ->setFont(Plan->Fonts[Fonts::Tree]);
  ui->taskWidget       ->setFont(Plan->Fonts[Fonts::Tree]);
  ui->childWidget      ->setFont(Plan->Fonts[Fonts::Tree]);
  ui->standaloneWidget ->setFont(Plan->Fonts[Fonts::Tree]);
  ui->predecessorWidget->setFont(Plan->Fonts[Fonts::Tree]);
  ui->successorWidget  ->setFont(Plan->Fonts[Fonts::Tree]);
  ui->frameworkBox     ->setFont(Plan->Fonts[Fonts::ComboBox]);
  ui->predecessorBox   ->setFont(Plan->Fonts[Fonts::ComboBox]);
  ui->successorBox     ->setFont(Plan->Fonts[Fonts::ComboBox]);
  ui->statusBox        ->setFont(Plan->Fonts[Fonts::ComboBox]);
  ui->fwFrameworkBox   ->setFont(Plan->Fonts[Fonts::ComboBox]);
  ui->fwPredecessorBox ->setFont(Plan->Fonts[Fonts::ComboBox]);
  ui->fwSuccessorBox   ->setFont(Plan->Fonts[Fonts::ComboBox]);
  ui->fwStatusBox      ->setFont(Plan->Fonts[Fonts::ComboBox]);
  ui->dnFrameworkBox   ->setFont(Plan->Fonts[Fonts::ComboBox]);
  ui->dnStatusBox      ->setFont(Plan->Fonts[Fonts::ComboBox]);
  ui->candidateBox     ->setFont(Plan->Fonts[Fonts::ComboBox]);
}

void TaskManager::Initialize(void)
{
  if (Project>0) {
    SetProject(Project);
    return;
  };
  Editor->Tasks = Tasks ;
  Editor->Plan  = Plan  ;
  Editor->Texts = Texts ;
  setCurrentWidget(ui->Projects);
  setFonts();
  LoadProjects();
}

void TaskManager::SetProject(SUID uuid)
{
  if (uuid==0) {
    Initialize();
    return;
  };
  Editor->Tasks = Tasks ;
  Editor->Plan  = Plan  ;
  Editor->Texts = Texts ;
  Project       = uuid  ;
  setCurrentWidget(ui->Lists);
  setFonts();
  LoadTasks(uuid);
}

void TaskManager::LoadProjects(void)
{
  ui->projectWidget->clear();
  Mutex.lock()       ;
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","LoadProjects");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    QString   Q      ;
    UUIDs     Uuids  ;
    SUID      uuid   ;
    Q = Sql.SelectFrom("uuid",Plan->Tables[Tables::Project]) +
            " where type = " + QString::number(Project::TASK) +
            " order by ltime desc;";
    nSqlUuids(q,Q,Uuids,0);
    foreach (uuid,Uuids) {
      QString           name  = ""   ;
      QString           pname = ""   ;
      int               tasks = 0    ;
      SUID              puid  = 0    ;
      QTreeWidgetItem * item  = NULL ;
      Q = Sql.SelectFrom("parent",Plan->Tables[Tables::ProjectMaps]) +
          " where uuid = " + QString::number(uuid) + ";";
      SqlGetOne(q,Q) puid = nSqlUuid(q,0);
      Q = Sql.SelectFrom("name",Plan->Tables[Tables::ProjectNames]) +
            " where uuid = " + QString::number(uuid) +
          " and language = " + QString::number(Plan->LanguageId) + ";";
      SqlGetOne(q,Q) name = SqlString(q,0);
      if (puid>0) {
        Q = Sql.SelectFrom("name",Plan->Tables[Tables::ProjectNames]) +
              " where uuid = " + QString::number(puid) +
            " and language = " + QString::number(Plan->LanguageId) + ";";
        SqlGetOne(q,Q) pname = SqlString(q,0);
      };
      Q = Sql.SelectFrom("count(*)",Plan->Tables[Tables::TaskMaps]) +
          " where uuid = " + QString::number(uuid) + ";";
      SqlGetOne(q,Q) tasks = q.value(0).toInt();
      item = new QTreeWidgetItem();
      item->setData(0,Qt::UserRole,uuid);
      item->setText(0,name);
      item->setData(1,Qt::UserRole,puid);
      item->setText(1,pname);
      item->setText(2,QString::number(tasks));
      ui->projectWidget->addTopLevelItem(item);
    };
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock()     ;
  ui->projectWidget->resizeColumnToContents(0);
  ui->projectWidget->resizeColumnToContents(1);
}

void TaskManager::projectSelected(QTreeWidgetItem * item,int column)
{
  SUID puid = item->data(0,Qt::UserRole).toULongLong();
  removeProjectEdits();
  switch (column) {
    case 0:
    case 2:
      ui->deleteProjectButton->setEnabled(puid>0);
    break;
    case 1:
    {
      ItemEditing = item ;
      Selector = new QComboBox(ui->projectWidget);
      ui->projectWidget->setItemWidget(ItemEditing,1,Selector);
      Mutex.lock();
      nSqlSettings Sql = Plan->Sql ;
      QString      CNN = Sql.connection("TaskManager","projectClicked");
      nSqlNow(CNN,db,Sql);
        QSqlQuery q (db) ;
        QString   Q      ;
        UUIDs     Uuids  ;
        SUID      puid   ;
        SUID      tuid  = ItemEditing->data(1,Qt::UserRole).toULongLong();
        int       index = -1 ;
        Q = Sql.SelectFrom("uuid",Plan->Tables[Tables::Project]) +
                " where type = " + QString::number(Project::MAJORPROJECT) +
                   " or type = " + QString::number(Project::SUBPROJECT) +
                   " or type = " + QString::number(Project::SCHEDULER) +
                " order by ltime desc;";
        nSqlUuids(q,Q,Uuids,0);
        foreach (puid,Uuids) {
          QString name = "" ;
          Q = Sql.SelectFrom("name",Plan->Tables[Tables::ProjectNames]) +
                " where uuid = " + QString::number(puid) +
              " and language = " + QString::number(Plan->LanguageId) + ";";
          SqlGetOne(q,Q) name = SqlString(q,0);
          if (tuid==puid) index = Selector->count();
          if (name.length()>0)
            Selector->addItem(name,puid);
        }
        if (index>=0) Selector->setCurrentIndex(index);
      nSqlErr(CNN,db,Sql);
      nSqlEnd(CNN,db,Sql);
      Mutex.unlock();
      Selector->showPopup();
      Selector->setFocus(Qt::TabFocusReason);
      Plan->processEvents();
      connect(Selector,SIGNAL(activated(int)),this,SLOT(subjectionFinished(int)));
    }
    break;
  };
}

void TaskManager::projectClicked(QTreeWidgetItem * item,int column)
{
  SUID uuid = item->data(0,Qt::UserRole).toULongLong();
  switch (column) {
    case 0: SetProject(uuid); break;
  };
}

void TaskManager::LoadTasks(SUID uuid)
{
  if (IsNull(Tasks)) Tasks = new nTasks();
  ui->taskWidget->clear();
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","LoadTasks");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    QString   Q      ;
    Q = Sql.SelectFrom("name",Plan->Tables[Tables::ProjectNames]) +
            " where uuid = " + QString::number(uuid) +
          " and language = " + QString::number(Plan->LanguageId) + ";";
    SqlGetOne(q,Q) setWindowTitle(SqlString(q,0));
    Tasks->load(Plan,q,uuid);
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  Tasks->list(ui->taskWidget);
  Plan->Talk(windowTitle());
}

void TaskManager::taskClicked(QTreeWidgetItem * item,int)
{
  if (IsNull(Tasks)) return;
  SUID      uuid = item->data(0,Qt::UserRole).toULongLong();
  showTaskDetails(uuid);
}

void TaskManager::showTaskDetails(SUID uuid)
{
  QWidget * w = ui->Undecided ;
  if (Tasks->Behaviors.contains(uuid)) {
    switch (Tasks->Behaviors[uuid]) {
      case Tasks::Undecided  :
        w = ui->Undecided  ;
        ui->undecidedButton->setChecked(true);
        ui->updateButton->setEnabled(false);
      break;
      case Tasks::Standalone :
        Disconnect();
        w = ui->Standalone ;
        ui->updateButton->setEnabled(false);
        ui->saObsoletekBox->setChecked(false);
        AddStatus(ui->statusBox,Tasks->Status[uuid]);
        Tasks->listFramework(ui->frameworkBox,uuid);
        ui->joinFrameworkBox->setEnabled(ui->frameworkBox->count()>0);
        ui->joinFrameworkBox->setChecked(Tasks->isFramework(uuid));
        ui->frameworkBox->setEnabled(Tasks->isFramework(uuid));
        ui->rootBox->setEnabled(Tasks->Parents.contains(uuid));
        ui->rootBox->setChecked(Tasks->Roots[uuid]);
        Tasks->listPredecessors(ui->predecessorBox,uuid);
        ui->prerequisteBox->setEnabled(ui->predecessorBox->count()>0);
        ui->prerequisteBox->setChecked(Tasks->isPredecessor(uuid));
        ui->predecessorBox->setEnabled(Tasks->isPredecessor(uuid));
        Tasks->listSuccessors(ui->successorBox,uuid);
        ui->heirBox->setEnabled(ui->successorBox->count()>0);
        ui->heirBox->setChecked(Tasks->isSuccessor(uuid));
        ui->successorBox->setEnabled(Tasks->isSuccessor(uuid));
        Connect();
      break;
      case Tasks::Framework  :
        Disconnect();
        w = ui->Framework  ;
        ui->updateButton->setEnabled(false);
        ui->fwObsoletekBox->setChecked(false);
        AddStatus(ui->fwStatusBox,Tasks->Status[uuid]);
        Tasks->listFramework(ui->fwFrameworkBox,uuid);
        ui->fwJoinFrameworkBox->setEnabled(ui->fwFrameworkBox->count()>0);
        ui->fwJoinFrameworkBox->setChecked(Tasks->isFramework(uuid));
        ui->fwFrameworkBox->setEnabled(Tasks->isFramework(uuid));
        ui->fwRootBox->setEnabled(Tasks->Parents.contains(uuid));
        ui->fwRootBox->setChecked(Tasks->Roots[uuid]);
        Tasks->listPredecessors(ui->fwPredecessorBox,uuid);
        ui->fwPrerequisteBox->setEnabled(ui->fwPredecessorBox->count()>0);
        ui->fwPrerequisteBox->setChecked(Tasks->isPredecessor(uuid));
        ui->fwPredecessorBox->setEnabled(Tasks->isPredecessor(uuid));
        Tasks->listSuccessors(ui->fwSuccessorBox,uuid);
        ui->fwHeirBox->setEnabled(ui->fwSuccessorBox->count()>0);
        ui->fwHeirBox->setChecked(Tasks->isSuccessor(uuid));
        ui->fwSuccessorBox->setEnabled(Tasks->isSuccessor(uuid));
        Tasks->listHierarchies(ui->standaloneWidget,uuid);
        Tasks->listFramework(ui->childWidget,uuid);
        Connect();
      break;
      case Tasks::Decision   :
        w = ui->Decision   ;
        ui->updateButton->setEnabled(false);
        ui->dnObsoleteBox->setChecked(false);
        AddStatus(ui->dnStatusBox,Tasks->Status[uuid]);
        Tasks->listFramework(ui->dnFrameworkBox,uuid);
        ui->dnJoinFrameworkBox->setEnabled(ui->dnFrameworkBox->count()>0);
        ui->dnJoinFrameworkBox->setChecked(Tasks->isFramework(uuid));
        ui->dnFrameworkBox->setEnabled(Tasks->isFramework(uuid));
        ui->dnRootBox->setEnabled(Tasks->Parents.contains(uuid));
        ui->dnRootBox->setChecked(Tasks->Roots[uuid]);
        Tasks->listCandidates(ui->candidateBox,uuid);
        Tasks->decisionPredecessor(ui->predecessorWidget,uuid);
        Tasks->decisionSuccessor(ui->successorWidget,uuid);
        Connect();
      break;
      case Tasks::Obsolete   :
        w = ui->Obsolete   ;
        ui->updateButton->setEnabled(false);
        ui->ActivateBox->setChecked(false);
      break;
    };
  };
  ui->taskProperty->setCurrentWidget(w);
}

void TaskManager::Edit(void)
{
  Editor->Tasks  = Tasks    ;
  Editor->Plan   = Plan     ;
  Editor->Texts  = Texts    ;
  Editor->FontId = FontId   ;
  setCurrentWidget (Editor) ;
  Editor->Prepare ()        ;
}

void TaskManager::removeProjectEdits(void)
{
  if (NotNull(ItemEditing)) {
    if (NotNull(NameEditor)) ui->projectWidget->removeItemWidget(ItemEditing,0);
    if (NotNull(Selector  )) ui->projectWidget->removeItemWidget(ItemEditing,1);
    if (ItemEditing->data(0,Qt::UserRole)==0)
      ui->projectWidget->takeTopLevelItem(ui->projectWidget->indexOfTopLevelItem(ItemEditing));
  };
  NameEditor  = NULL ;
  Selector    = NULL ;
  ItemEditing = NULL ;
}

void TaskManager::projectFinished(void)
{
  if (IsNull(NameEditor)) return;
  QString name = NameEditor->text();
  SUID    puid = 0;
  if (name.length()<=0) {
    removeProjectEdits();
    return;
  };
  ui->projectWidget->removeItemWidget(ItemEditing,0);
  NameEditor = NULL;
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","projectFinished");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    QString   Q      ;
    puid = UniqueUuid(Sql,q,Plan->Tables[Tables::MajorUuid],"uuid");
    if (puid>0) {
      SqlQuick::insertUuid(Sql,q,Plan->Tables[Tables::MajorUuid],puid,"uuid");
      Q = Sql.InsertInto(
            "uuid,type",
            Plan->Tables[Tables::Project],
            QString("%1,%2").arg(puid).arg(Project::TASK)
          ) + ";" ;
      q.exec(Q);
      Q = Sql.ReplaceInto(
            " uuid, language, name",
            Plan->Tables[Tables::ProjectNames],
            ":UUID,:LANGUAGE,:NAME"
          ) + ";";
      q.prepare(Q);
      SqlBind(q,":UUID",puid);
      SqlBind(q,":LANGUAGE",Plan->LanguageId);
      SqlBind(q,":NAME",name.toUtf8());
      q.exec();
    };
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  if (puid>0) {
    ItemEditing->setText(0,name);
    ItemEditing->setData(0,Qt::UserRole,puid);
    ItemEditing->setData(1,Qt::UserRole,0);
    ItemEditing = NULL;
  } else {
    removeProjectEdits();
  };
}

void TaskManager::subjectionFinished(int index)
{
  if (IsNull(ItemEditing)) {
    if (NotNull(Selector)) Selector->deleteLater();
    Selector = NULL;
    return ;
  };
  SUID puid = Selector->itemData(index,Qt::UserRole).toULongLong();
  SUID uuid = ItemEditing->data(0,Qt::UserRole).toULongLong();
  if (puid<=0 || uuid<=0) {
    if (NotNull(Selector)) ui->projectWidget->removeItemWidget(ItemEditing,1);
    Selector = NULL;
    ItemEditing = NULL;
    return;
  };
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","subjectionFinished");
  nSqlNow(CNN,db,Sql)   ;
    QSqlQuery q (db)    ;
    QString   Q         ;
    Q = Sql.ReplaceInto(
          "uuid,parent",
          Plan->Tables[Tables::ProjectMaps],
          QString("%1,%2").arg(uuid).arg(puid)
        ) + ";" ;
    q.exec(Q);
    Q = Sql.SelectFrom("name",Plan->Tables[Tables::ProjectNames]) +
            " where uuid = " + QString::number(puid) +
          " and language = " + QString::number(Plan->LanguageId) + ";";
    SqlGetOne(q,Q) ItemEditing->setText(1,SqlString(q,0));
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  ItemEditing->setData(1,Qt::UserRole,puid);
  ui->projectWidget->removeItemWidget(ItemEditing,1);
  Selector = NULL;
  ItemEditing = NULL;
}

void TaskManager::AddProject(void)
{
  removeProjectEdits();
  ItemEditing = new QTreeWidgetItem();
  ui->projectWidget->insertTopLevelItem(0,ItemEditing);
  ui->projectWidget->scrollToItem(ItemEditing);
  ItemEditing->setData(0,Qt::UserRole,0);
  NameEditor = new QLineEdit(ui->projectWidget);
  NameEditor->setFont(font());
  ui->projectWidget->setItemWidget(ItemEditing,0,NameEditor);
  connect(NameEditor,SIGNAL(editingFinished()),this,SLOT(projectFinished()));
  NameEditor->setFocus(Qt::TabFocusReason);
}

void TaskManager::taskNameFinished(void)
{
  if (IsNull(NameEditor)) return;
  QString name = NameEditor->text();
  if (name.length()<=0) {
    removeProjectEdits();
    return;
  };
  SUID    tuid = 0;
  ui->taskWidget->removeItemWidget(ItemEditing,0);
  NameEditor = NULL;
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","taskNameFinished");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    QString   Q      ;
    tuid = UniqueUuid(Sql,q,Plan->Tables[Tables::MajorUuid],"uuid");
    if (tuid>0) {
      SqlQuick::insertUuid(Sql,q,Plan->Tables[Tables::MajorUuid],tuid,"uuid");
      SqlQuick::insertUuid(Sql,q,Plan->Tables[Tables::Tasks],tuid,"uuid");
      Q = Sql.ReplaceInto(
            " uuid, language, name",
            Plan->Tables[Tables::TaskNames],
            ":UUID,:LANGUAGE,:NAME"
          ) + ";";
      q.prepare(Q);
      SqlBind(q,":UUID",tuid);
      SqlBind(q,":LANGUAGE",Plan->LanguageId);
      SqlBind(q,":NAME",name.toUtf8());
      q.exec();
      Q = Sql.InsertInto(
            "uuid,task",
            Plan->Tables[Tables::TaskMaps],
            QString("%1,%2").arg(Project).arg(tuid)
          ) + ";" ;
      q.exec(Q);
      Q = Sql.ReplaceInto(
            "uuid,behavior,status,flags",
            Plan->Tables[Tables::TaskFlags],
            QString("%1,%2,%3,0")
                  .arg(tuid)
                  .arg(Tasks::Undecided)
                  .arg(Tasks::Template)
          ) + ";";
      q.exec(Q);
    };
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  if (tuid>0) {
    ItemEditing->setText(0,name);
    ItemEditing->setData(0,Qt::UserRole,tuid);
    Tasks->Tasks << tuid ;
    Tasks->Behaviors[tuid] = Tasks::Undecided ;
    Tasks->Names    [tuid] = name;
    Tasks->setIcon(ItemEditing,Tasks::Undecided);
    ItemEditing = NULL;
    showTaskDetails(tuid);
  } else {
    removeProjectEdits();
  };
  emit EventSignal(Command::Done);
}

void TaskManager::AddTask(void)
{
  removeProjectEdits();
  ItemEditing = new QTreeWidgetItem();
  ui->taskWidget->insertTopLevelItem(0,ItemEditing);
  ui->taskWidget->scrollToItem(ItemEditing);
  ItemEditing->setData(0,Qt::UserRole,0);
  NameEditor = new QLineEdit(ui->taskWidget);
  NameEditor->setFont(font());
  ui->taskWidget->setItemWidget(ItemEditing,0,NameEditor);
  connect(NameEditor,SIGNAL(editingFinished()),this,SLOT(taskNameFinished()));
  NameEditor->setFocus(Qt::TabFocusReason);
}

void TaskManager::undecidedBehavior(bool clicked)
{
  QTreeWidgetItem * it = ui->taskWidget->currentItem();
  bool enable;
  enable = !ui->undecidedButton->isChecked();
  if (IsNull(it)) enable = false;
  ui->updateButton->setEnabled(enable);
}

void TaskManager::UpdateBehavior(void)
{
  QTreeWidgetItem * it = ui->taskWidget->currentItem();
  SUID tuid = it->data(0,Qt::UserRole).toULongLong();
  int behavior = Tasks::Undecided ;
  if (Tasks->Behaviors.contains(tuid)) behavior = Tasks->Behaviors[tuid];
  QWidget * w = ui->taskProperty->currentWidget();
  switch (behavior) {
    case Tasks::Undecided  :
      if (w==ui->Undecided) updateUndecided(it,tuid);
    break;
    case Tasks::Standalone :
      if (w==ui->Standalone) updateStandalone(it,tuid);
    break;
    case Tasks::Framework  :
      if (w==ui->Framework) updateFramework(it,tuid);
    break;
    case Tasks::Decision   :
      if (w==ui->Decision) updateDecision(it,tuid);
    break;
    case Tasks::Obsolete   :
      if (w==ui->Obsolete) updateObsolete(it,tuid);
    break;
  };
}

int TaskManager::decideBehavior(void)
{
  if (ui->standaloneButton->isChecked()) return Tasks::Standalone ;
  if (ui->frameworkButton ->isChecked()) return Tasks::Framework  ;
  if (ui->decisionButton  ->isChecked()) return Tasks::Decision   ;
  if (ui->obsoleteButton  ->isChecked()) return Tasks::Obsolete   ;
  return Tasks::Undecided                                         ;
}

int TaskManager::obsoleteBehavior(void)
{
  if (ui->standaloneObsoleteButton->isChecked()) return Tasks::Standalone ;
  if (ui->frameworkObsoleteButton ->isChecked()) return Tasks::Framework  ;
  if (ui->decisionObsoleteButton  ->isChecked()) return Tasks::Decision   ;
  if (ui->obsoleteObsoleteButton  ->isChecked()) return Tasks::Obsolete   ;
  return Tasks::Undecided                                                 ;
}

void TaskManager::updateUndecided(QTreeWidgetItem * item,SUID uuid)
{
  int behavior = decideBehavior();
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","updateUndecided");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    QString   Q      ;
    Q = Sql.ReplaceInto(
          "uuid,behavior",
          Plan->Tables[Tables::TaskFlags],
          QString("%1,%2").arg(uuid).arg(behavior)
        ) + ";";
    q.exec(Q);
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Tasks->Behaviors[uuid] = behavior ;
  Mutex.unlock();
  Tasks->setIcon(item,behavior);
  showTaskDetails(uuid);
}

void TaskManager::updateStandalone(QTreeWidgetItem * item,SUID uuid)
{
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","updateStandalone");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    QString   Q      ;
    if (ui->saObsoletekBox->isChecked()) {
      Tasks->Behaviors[uuid] = Tasks::Obsolete ;
      Q = Sql.ReplaceInto(
            "uuid,behavior",
            Plan->Tables[Tables::TaskFlags],
            QString("%1,%2").arg(uuid).arg(Tasks::Obsolete)
          ) + ";";
      q.exec(Q);
      Tasks->setIcon(item,Tasks::Obsolete);
    };
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  showTaskDetails(uuid);
}

void TaskManager::updateFramework(QTreeWidgetItem * item,SUID uuid)
{
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","updateFramework");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    QString   Q      ;
    if (ui->fwObsoletekBox->isChecked()) {
      Tasks->Behaviors[uuid] = Tasks::Obsolete ;
      Q = Sql.ReplaceInto(
            "uuid,behavior",
            Plan->Tables[Tables::TaskFlags],
            QString("%1,%2").arg(uuid).arg(Tasks::Obsolete)
          ) + ";";
      q.exec(Q);
      Tasks->setIcon(item,Tasks::Obsolete);
    };
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  showTaskDetails(uuid);
}

void TaskManager::updateDecision(QTreeWidgetItem * item,SUID uuid)
{
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","updateDecision");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    QString   Q      ;
    if (ui->dnObsoleteBox->isChecked()) {
      Tasks->Behaviors[uuid] = Tasks::Obsolete ;
      Q = Sql.ReplaceInto(
            "uuid,behavior",
            Plan->Tables[Tables::TaskFlags],
            QString("%1,%2").arg(uuid).arg(Tasks::Obsolete)
          ) + ";";
      q.exec(Q);
      Tasks->setIcon(item,Tasks::Obsolete);
    };
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  showTaskDetails(uuid);
}

void TaskManager::updateObsolete(QTreeWidgetItem * item,SUID uuid)
{
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","updateObsolete");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    QString   Q      ;
    if (ui->ActivateBox->isChecked()) {
      Tasks->Behaviors[uuid] = obsoleteBehavior();
      Q = Sql.ReplaceInto(
            "uuid,behavior",
            Plan->Tables[Tables::TaskFlags],
            QString("%1,%2").arg(uuid).arg(Tasks->Behaviors[uuid])
          ) + ";";
      q.exec(Q);
      Tasks->setIcon(item,Tasks->Behaviors[uuid]);
    };
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  showTaskDetails(uuid);
}

void TaskManager::AddStatus(QComboBox * combo,int status)
{
  combo->clear()                                                 ;
  combo->addItem(tr("Template"   ),Tasks::Template   ) ;
  combo->addItem(tr("Proposed"   ),Tasks::Proposed   ) ;
  combo->addItem(tr("Analysis"   ),Tasks::Analysis   ) ;
  combo->addItem(tr("Preparation"),Tasks::Preparation) ;
  combo->addItem(tr("Ready"      ),Tasks::Ready      ) ;
  combo->addItem(tr("Dispatching"),Tasks::Dispatching) ;
  combo->addItem(tr("Assigned"   ),Tasks::Assigned   ) ;
  combo->addItem(tr("Execution"  ),Tasks::Execution  ) ;
  combo->addItem(tr("Feedback"   ),Tasks::Feedback   ) ;
  combo->addItem(tr("Completed"  ),Tasks::Completed  ) ;
  combo->addItem(tr("Terminated" ),Tasks::Terminated ) ;
  combo->addItem(tr("Expired"    ),Tasks::Expired    ) ;
  combo->addItem(tr("Forwarded"  ),Tasks::Forwarded  ) ;
  combo->addItem(tr("Finished"   ),Tasks::Finished   ) ;
  combo->addItem(tr("Failed"     ),Tasks::Failed     ) ;
  combo->addItem(tr("Delegated"  ),Tasks::Delegated  ) ;
  combo->addItem(tr("Rejected"   ),Tasks::Rejected   ) ;
  combo->addItem(tr("Review"     ),Tasks::Review     ) ;
  combo->setCurrentIndex(status)                                 ;
}

void TaskManager::statusChanged(int index)
{
  int status = Tasks::Template ;
  QTreeWidgetItem * item = ui->taskWidget->currentItem();
  if (IsNull(item)) return;
  SUID uuid = item->data(0,Qt::UserRole).toULongLong();
  if (uuid<=0) return;
  int behavior = Tasks->Behaviors[uuid] ;
  switch (behavior) {
    case Tasks::Standalone:
      status = ui->statusBox->itemData(index,Qt::UserRole).toInt();
    break;
    case Tasks::Framework:
      status = ui->fwStatusBox->itemData(index,Qt::UserRole).toInt();
    break;
    case Tasks::Decision:
      status = ui->dnStatusBox->itemData(index,Qt::UserRole).toInt();
    break;
    default : return;
  };
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","statusChanged");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    QString   Q      ;
    Q = Sql.Update(Plan->Tables[Tables::TaskFlags]) +
        " status = " + QString::number(status) +
        " where uuid = " + QString::number(uuid) + ";";
    q.exec(Q);
    Q = Sql.SelectFrom("status",Plan->Tables[Tables::TaskFlags]) +
        " where uuid = " + QString::number(uuid) + ";";
    SqlGetOne(q,Q) Tasks->Status[uuid] = q.value(0).toInt();
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  emit EventSignal(Command::Action);
}

void TaskManager::joinFramework(bool)
{
  QTreeWidgetItem * item = ui->taskWidget->currentItem();
  if (IsNull(item)) return;
  SUID uuid = item->data(0,Qt::UserRole).toULongLong();
  if (uuid<=0) return;
  int behavior = Tasks->Behaviors[uuid] ;
  bool canJoin = false ;
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","joinFramework");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    switch (behavior) {
      case Tasks::Standalone:
        canJoin = ui->joinFrameworkBox->isChecked();
        ui->frameworkBox->setEnabled(canJoin);
        if (canJoin) {
          ui->rootBox->setEnabled(ui->frameworkBox->count());
        } else {
          ui->rootBox->setEnabled(false);
          Tasks->detachFrameWork(Plan,q,uuid);
        };
      break;
      case Tasks::Framework:
        canJoin = ui->fwJoinFrameworkBox->isChecked();
        ui->fwFrameworkBox->setEnabled(canJoin);
        if (canJoin) {
          ui->fwRootBox->setEnabled(ui->fwFrameworkBox->count());
        } else {
          ui->fwRootBox->setEnabled(false);
          Tasks->detachFrameWork(Plan,q,uuid);
        };
        Tasks->listHierarchies(ui->standaloneWidget,uuid);
        Tasks->listFramework(ui->childWidget,uuid);
      break;
      case Tasks::Decision:
        canJoin = ui->dnJoinFrameworkBox->isChecked();
        ui->dnFrameworkBox->setEnabled(canJoin);
        if (canJoin) {
          ui->dnRootBox->setEnabled(ui->dnFrameworkBox->count());
        } else {
          ui->dnRootBox->setEnabled(false);
          Tasks->detachFrameWork(Plan,q,uuid);
        };
      break;
      default : return;
    };
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  emit EventSignal(Command::Action);
}

void TaskManager::setFramework(int)
{
  QTreeWidgetItem * item = ui->taskWidget->currentItem();
  if (IsNull(item)) return;
  SUID uuid = item->data(0,Qt::UserRole).toULongLong();
  if (uuid<=0) return;
  int behavior = Tasks->Behaviors[uuid] ;
  bool canJoin = false ;
  bool isRoot  = false ;
  SUID puid = 0;
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","setFramework");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    switch (behavior) {
      case Tasks::Standalone:
        canJoin = ui->joinFrameworkBox->isChecked();
        isRoot  = ui->rootBox->isChecked();
        puid = ui->frameworkBox->itemData(ui->frameworkBox->currentIndex(),Qt::UserRole).toULongLong();
        if (canJoin) {
          Tasks->joinFrameWork(Plan,q,uuid,puid,isRoot);
        } else {
          Tasks->detachFrameWork(Plan,q,uuid);
        };
      break;
      case Tasks::Framework:
        canJoin = ui->fwJoinFrameworkBox->isChecked();
        isRoot  = ui->fwRootBox->isChecked();
        puid = ui->fwFrameworkBox->itemData(ui->fwFrameworkBox->currentIndex(),Qt::UserRole).toULongLong();
        if (canJoin) {
          Tasks->joinFrameWork(Plan,q,uuid,puid,isRoot);
        } else {
          Tasks->detachFrameWork(Plan,q,uuid);
        };
        Tasks->listHierarchies(ui->standaloneWidget,uuid);
        Tasks->listFramework(ui->childWidget,uuid);
      break;
      case Tasks::Decision:
        canJoin = ui->dnJoinFrameworkBox->isChecked();
        isRoot  = ui->dnRootBox->isChecked();
        puid = ui->dnFrameworkBox->itemData(ui->dnFrameworkBox->currentIndex(),Qt::UserRole).toULongLong();
        if (canJoin) {
          Tasks->joinFrameWork(Plan,q,uuid,puid,isRoot);
        } else {
          Tasks->detachFrameWork(Plan,q,uuid);
        };
      break;
      default : return;
    };
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  emit EventSignal(Command::Action);
}

void TaskManager::joinThisFramework(void)
{
  QTreeWidgetItem * item = ui->taskWidget->currentItem();
  if (IsNull(item)) return;
  SUID uuid = item->data(0,Qt::UserRole).toULongLong();
  if (uuid<=0) return;
  QTreeWidgetItem * ptem = ui->standaloneWidget->currentItem();
  if (IsNull(ptem)) return;
  SUID puid = ptem->data(0,Qt::UserRole).toULongLong();
  if (puid<=0) return;
  bool isRoot = ui->fwRootBox->isChecked();
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","joinThisFramework");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    Tasks->joinFrameWork(Plan,q,puid,uuid,isRoot);
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  Tasks->listHierarchies(ui->standaloneWidget,uuid);
  Tasks->listFramework(ui->childWidget,uuid);
  emit EventSignal(Command::Action);
}

void TaskManager::removeFromFramework(void)
{
  QTreeWidgetItem * item = ui->taskWidget->currentItem();
  if (IsNull(item)) return;
  SUID uuid = item->data(0,Qt::UserRole).toULongLong();
  if (uuid<=0) return;
  QTreeWidgetItem * ptem = ui->childWidget->currentItem();
  if (IsNull(ptem)) return;
  SUID puid = ptem->data(0,Qt::UserRole).toULongLong();
  if (puid<=0) return;
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","removeFromFramework");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    Tasks->detachFrameWork(Plan,q,puid);
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  Tasks->listHierarchies(ui->standaloneWidget,uuid);
  Tasks->listFramework(ui->childWidget,uuid);
  emit EventSignal(Command::Action);
}

void TaskManager::setPredecessor(bool)
{
  QTreeWidgetItem * item = ui->taskWidget->currentItem();
  if (IsNull(item)) return;
  SUID uuid = item->data(0,Qt::UserRole).toULongLong();
  if (uuid<=0) return;
  Mutex.lock();
  int behavior = Tasks->Behaviors[uuid] ;
  bool canJoin = false ;
  SUID tuid = 0;
  SUID puid = 0;
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","setPredecessor");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    switch (behavior) {
      case Tasks::Standalone:
        canJoin = ui->prerequisteBox->isChecked();
        ui->predecessorBox->setEnabled(canJoin);
        tuid = ui->predecessorBox->itemData(ui->predecessorBox->currentIndex(),Qt::UserRole).toULongLong();
        puid = 0;
        if (Tasks->Predecessors.contains(uuid)) puid = Tasks->Predecessors[uuid];
        if (canJoin) {
        } else {
          Tasks->updatePredecessor(Plan,q,uuid,0,puid);
        };
      break;
      case Tasks::Framework:
        canJoin = ui->fwPrerequisteBox->isChecked();
        ui->fwPredecessorBox->setEnabled(canJoin);
        tuid = ui->fwPredecessorBox->itemData(ui->fwPredecessorBox->currentIndex(),Qt::UserRole).toULongLong();
        puid = 0;
        if (Tasks->Predecessors.contains(uuid)) puid = Tasks->Predecessors[uuid];
        if (canJoin) {
        } else {
          Tasks->updatePredecessor(Plan,q,uuid,0,puid);
        };
      break;
      default : return;
    };
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  emit EventSignal(Command::Action);
}

void TaskManager::PredecessorChanged(int)
{
  QTreeWidgetItem * item = ui->taskWidget->currentItem();
  if (IsNull(item)) return;
  SUID uuid = item->data(0,Qt::UserRole).toULongLong();
  if (uuid<=0) return;
  Mutex.lock();
  int behavior = Tasks->Behaviors[uuid] ;
  bool canJoin = false ;
  SUID tuid = 0;
  SUID puid = 0;
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","PredecessorChanged");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    switch (behavior) {
      case Tasks::Standalone:
        canJoin = ui->prerequisteBox->isChecked();
        tuid = ui->predecessorBox->itemData(ui->predecessorBox->currentIndex(),Qt::UserRole).toULongLong();
        puid = 0;
        if (Tasks->Predecessors.contains(uuid)) puid = Tasks->Predecessors[uuid];
        if (canJoin) {
          Tasks->updatePredecessor(Plan,q,uuid,tuid,puid);
        } else {
          Tasks->updatePredecessor(Plan,q,uuid,0,puid);
        };
      break;
      case Tasks::Framework:
        canJoin = ui->fwPrerequisteBox->isChecked();
        tuid = ui->fwPredecessorBox->itemData(ui->fwPredecessorBox->currentIndex(),Qt::UserRole).toULongLong();
        puid = 0;
        if (Tasks->Predecessors.contains(uuid)) puid = Tasks->Predecessors[uuid];
        if (canJoin) {
          Tasks->updatePredecessor(Plan,q,uuid,tuid,puid);
        } else {
          Tasks->updatePredecessor(Plan,q,uuid,0,puid);
        };
      break;
      default : return;
    };
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  emit EventSignal(Command::Action);
}

void TaskManager::setSuccessor(bool)
{
  QTreeWidgetItem * item = ui->taskWidget->currentItem();
  if (IsNull(item)) return;
  SUID uuid = item->data(0,Qt::UserRole).toULongLong();
  if (uuid<=0) return;
  Mutex.lock();
  int behavior = Tasks->Behaviors[uuid] ;
  bool canJoin = false ;
  SUID tuid = 0;
  SUID puid = 0;
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","setSuccessor");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    switch (behavior) {
      case Tasks::Standalone:
        canJoin = ui->heirBox->isChecked();
        ui->successorBox->setEnabled(canJoin);
        tuid = ui->successorBox->itemData(ui->successorBox->currentIndex(),Qt::UserRole).toULongLong();
        puid = 0;
        if (Tasks->Successors.contains(uuid)) puid = Tasks->Successors[uuid];
        if (canJoin) {
        } else {
          Tasks->updateSuccessor(Plan,q,uuid,0,puid);
        };
      break;
      case Tasks::Framework:
        canJoin = ui->fwHeirBox->isChecked();
        ui->fwSuccessorBox->setEnabled(canJoin);
        tuid = ui->fwSuccessorBox->itemData(ui->fwSuccessorBox->currentIndex(),Qt::UserRole).toULongLong();
        puid = 0;
        if (Tasks->Successors.contains(uuid)) puid = Tasks->Successors[uuid];
        if (canJoin) {
        } else {
          Tasks->updateSuccessor(Plan,q,uuid,0,puid);
        };
      break;
      default : return;
    };
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  emit EventSignal(Command::Action);
}

void TaskManager::SuccessorChanged(int)
{
  QTreeWidgetItem * item = ui->taskWidget->currentItem();
  if (IsNull(item)) return;
  SUID uuid = item->data(0,Qt::UserRole).toULongLong();
  if (uuid<=0) return;
  Mutex.lock();
  int behavior = Tasks->Behaviors[uuid] ;
  bool canJoin = false ;
  SUID tuid = 0;
  SUID puid = 0;
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","SuccessorChanged");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    switch (behavior) {
      case Tasks::Standalone:
        canJoin = ui->heirBox->isChecked();
        tuid = ui->successorBox->itemData(ui->successorBox->currentIndex(),Qt::UserRole).toULongLong();
        puid = 0;
        if (Tasks->Successors.contains(uuid)) puid = Tasks->Successors[uuid];
        if (canJoin) {
          Tasks->updateSuccessor(Plan,q,uuid,tuid,puid);
        } else {
          Tasks->updateSuccessor(Plan,q,uuid,0,puid);
        };
      break;
      case Tasks::Framework:
        canJoin = ui->fwHeirBox->isChecked();
        tuid = ui->fwSuccessorBox->itemData(ui->fwSuccessorBox->currentIndex(),Qt::UserRole).toULongLong();
        puid = 0;
        if (Tasks->Successors.contains(uuid)) puid = Tasks->Successors[uuid];
        if (canJoin) {
          Tasks->updateSuccessor(Plan,q,uuid,tuid,puid);
        } else {
          Tasks->updateSuccessor(Plan,q,uuid,0,puid);
        };
      break;
      default : return;
    };
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  emit EventSignal(Command::Action);
}

void TaskManager::DecisionHierarchy(int index)
{
  if (index<0) return;
  SUID uuid = ui->candidateBox->itemData(index,Qt::UserRole).toULongLong();
  if (uuid<=0) return;
  ui->withdrawalButton->setEnabled(false);
  ui->predecessorWidget->setCurrentItem(NULL);
  ui->successorWidget->setCurrentItem(NULL);
  bool prerequisteEnable = false ;
  bool heirEnable        = false ;
  if (Tasks->Behaviors[uuid]==Tasks::Decision) {
    prerequisteEnable = true ;
    heirEnable        = true ;
  } else {
    prerequisteEnable = !Tasks->Successors   . contains(uuid) ;
    heirEnable        = !Tasks->Predecessors . contains(uuid) ;
  };
  if (ui->dnRootBox->isChecked()) prerequisteEnable = false ;
  ui->addPrerequisteButton->setEnabled(prerequisteEnable) ;
  ui->addHeirButton       ->setEnabled(heirEnable       ) ;
}

void TaskManager::AddPrerequiste(void)
{
  QTreeWidgetItem * item = ui->taskWidget->currentItem();
  if (IsNull(item)) return;
  SUID uuid = item->data(0,Qt::UserRole).toULongLong();
  if (uuid<=0) return;
  SUID puid = ui->candidateBox->itemData(ui->candidateBox->currentIndex(),Qt::UserRole).toULongLong();
  if (puid<=0) return;
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","SuccessorChanged");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    Tasks->addPredecessor(Plan,q,uuid,puid);
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  Tasks->listCandidates(ui->candidateBox,uuid);
  Tasks->decisionPredecessor(ui->predecessorWidget,uuid);
  Tasks->decisionSuccessor(ui->successorWidget,uuid);
  emit EventSignal(Command::Action);
}

void TaskManager::AddHeir(void)
{
  QTreeWidgetItem * item = ui->taskWidget->currentItem();
  if (IsNull(item)) return;
  SUID uuid = item->data(0,Qt::UserRole).toULongLong();
  if (uuid<=0) return;
  SUID puid = ui->candidateBox->itemData(ui->candidateBox->currentIndex(),Qt::UserRole).toULongLong();
  if (puid<=0) return;
  Mutex.lock();
  nSqlSettings Sql = Plan->Sql;
  QString      CNN = Sql.connection("TaskManager","SuccessorChanged");
  nSqlNow(CNN,db,Sql);
    QSqlQuery q (db) ;
    Tasks->addHeir(Plan,q,uuid,puid);
  nSqlErr(CNN,db,Sql);
  nSqlEnd(CNN,db,Sql);
  Mutex.unlock();
  Tasks->listCandidates(ui->candidateBox,uuid);
  Tasks->decisionPredecessor(ui->predecessorWidget,uuid);
  Tasks->decisionSuccessor(ui->successorWidget,uuid);
  emit EventSignal(Command::Action);
}

void TaskManager::PredecessorClicked(QTreeWidgetItem * item,int column)
{
  ui->successorWidget->setCurrentItem(NULL);
  ui->withdrawalButton->setEnabled(true);
}

void TaskManager::SuccessorClicked(QTreeWidgetItem * item,int column)
{
  ui->predecessorWidget->setCurrentItem(NULL);
  ui->withdrawalButton->setEnabled(true);
}
#endif
