PROJECT(org_mitk_planarfigure)

MACRO_CREATE_MITK_CTK_PLUGIN(
  EXPORT_DIRECTIVE ORG_MITK_PLANARFIGURE_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDENCIES PlanarFigure Qmitk
  )
