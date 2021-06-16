#include <qtvcf.h>

N::Contours N::Graphics::PathToContours (QPainterPath & path)
{
  N::Contours contours                     ;
  if (path.isEmpty()) return contours      ;
  int       total = path.elementCount()    ;
  if (total<=0      ) return contours      ;
  N::Contour             * c = NULL        ;
  QPainterPath::Element pe                 ;
  int                   position           ;
  int                   Type               ;
  QPointF               z                  ;
  Type = N::Graphics::Flat                 ;
  for (int i=0;i<total;i++)                {
    pe = path . elementAt ( i )            ;
    QPointF p ( pe.x , pe.y )              ;
    if (pe.isMoveTo ())                    {
      if (NotNull(c))                      {
        c->close(Type)                     ;
        contours << *c                     ;
        c = NULL                           ;
      }                                    ;
      position = 1                         ;
      Type = N::Graphics::Flat             ;
      N::ControlPoint cp                   ;
      cp = p                               ;
      cp . Type = Type                     ;
      c  = new N::Contour()                ;
      c  -> begin (               )        ;
      c  -> add   ( position , cp )        ;
    } else
    if (pe.isLineTo ())                    {
      Type = N::Graphics::Flat             ;
      position++                           ;
      N::ControlPoint cp                   ;
      cp = p                               ;
      cp . Type = Type                     ;
      c  -> add   ( position , cp )        ;
    } else
    if (pe.isCurveTo())                    {
      Type = N::Graphics::Quadratic        ;
      position++                           ;
      N::ControlPoint cp                   ;
      cp = p                               ;
      cp . Type = Type                     ;
      c  -> add   ( position , cp )        ;
    }                                      ;
  }                                        ;
  if (NotNull(c))                          {
    c->close(Type)                         ;
    contours << *c                         ;
    c = NULL                               ;
  }                                        ;
  return contours                          ;
}

QList<QPainterPath> N::Graphics::GlyphPathes (QFont & Font,QString message)
{
  QList<QPainterPath> Pathes                                 ;
  QRawFont         RF    = QRawFont::fromFont(Font)          ;
  QVector<quint32> glyph = RF.glyphIndexesForString(message) ;
  if (glyph.count()<=0) return Pathes                        ;
  for (int i=0;i<glyph.count();i++)                          {
    QPainterPath p = RF.pathForGlyph(glyph[i])               ;
    if (!p.isEmpty()) Pathes << p                            ;
  }                                                          ;
  return Pathes                                              ;
}

N::Contours N::Graphics::Glyph(QFont & Font,QString message)
{
  N::Contours         contours                           ;
  QList<QPainterPath> Pathes                             ;
  Pathes   = N::Graphics::GlyphPathes ( Font , message ) ;
  if (Pathes.count()<=0) return contours                 ;
  for (int i=0;i<Pathes.count();i++)                     {
    Contours c = N::Graphics::PathToContours(Pathes[i])  ;
    if (c.count()>0)                                     {
      for (int j=0;j<c.count();j++) contours << c [j]    ;
    }                                                    ;
  }                                                      ;
  return contours                                        ;
}
