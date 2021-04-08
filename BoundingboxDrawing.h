#pragma once

#include    <Mstn/MdlApi/MdlApi.h>
#include    <DgnView/DgnElementSetTool.h>
#include    <Bentley/stdcxx/bvector.h>
#include	<Mstn/isessionmgr.h>
#include	<DgnPlatform/LinearHandlers.h>
#include	<DgnPlatform/DisplayHandler.h>
#include	<DgnPlatform/MeshHeaderHandler.h>
#include	<Mstn/ElementPropertyUtils.h>


USING_NAMESPACE_BENTLEY_DGNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT


class BoundingboxDrawing : public DgnElementSetTool
{
public:
	BoundingboxDrawing();
	~BoundingboxDrawing();

	static void InstallNewInstance();
protected:
	virtual bool			_DoGroups() override { return false; }
	virtual bool			_NeedAcceptPoint() override { return SOURCE_Pick == _GetElemSource(); }
	virtual UsesDragSelect  _AllowDragSelect() override { return USES_DRAGSELECT_Box; }
	virtual bool			_NeedPointForSelection() override { return false; }
	virtual void			_OnRestartTool() override;
	virtual bool			_OnDataButton(DgnButtonEventCR ev) override;
	virtual void			_SetupAndPromptForNextAction() override;
	virtual StatusInt		_OnElementModify(EditElementHandleR eeh) override;
	virtual StatusInt		_ProcessAgenda(DgnButtonEventCR ev) override;
	virtual bool			_OnModifyComplete(DgnButtonEventCR ev) override;

private:
	void CalcLineBounding();
	void CreateBoundingLine();
	void CreateBounding3D();

	void CreateBoundingPoly(bvector<DPoint3d> points);
	void CreateBounding();
private:
	DRange3d m_rangeSum = DRange3d::NullRange();
};
