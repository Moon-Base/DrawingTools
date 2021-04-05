#pragma once

#include    <Mstn/MdlApi/MdlApi.h>
#include    <DgnView/DgnElementSetTool.h>
#include    <Bentley/stdcxx/bvector.h>
#include	<PSolid/PSolidCoreAPI.h>
#include	<DgnView/LocateSubEntityTool.h>
#include	<Mstn/isessionmgr.h>
#include <DgnPlatform\DgnPlatformAPI.h>


USING_NAMESPACE_BENTLEY_DGNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT



struct EleAgendaEvents : IElementAgendaEvents
{
private:
	virtual bool _OnRedrawGroupEvent(ElementAgendaCP, AgendaOperation, AgendaModify, RedrawGroupInfo const*) override;
};

struct BoundingboxTool : public DgnElementSetTool
{
public:
	
	static void  InstallNewInstance(int toolId);

private:
	enum class  BoundingboxCreateToolState
	{
		TOOLSTATE_Locating = 0,
		TOOLSTATE_Dynamics = 1
	};

	BoundingboxCreateToolState      m_toolState;
	int                             m_iView;
	HitPathCP                       m_hitPath;
	bvector<DPoint3d>				m_points;

	EditElementHandleP              m_editElementHandleP;
	int								m_ElementCount = 0;
	MSElementDescrCP				tranEdP = NULL;
	RotMatrixCP						rotMatrixP = NULL;

	BoundingboxTool(int toolName) : DgnElementSetTool(toolName) { }
	
	virtual		void					_OnPostInstall() override;
	//virtual		bool					_OnDataButton(DgnButtonEventCR ev) override;
	virtual		void					_OnRestartTool()override;
	//virtual     bool					_OnPostLocate(HitPathCP path, WStringR cantAcceptReason) override;
	virtual		UsesDragSelect			_AllowDragSelect()override { return USES_DRAGSELECT_Box; }
	virtual		void					_SetupAndPromptForNextAction();
	//virtual		void					_OnDynamicFrame(DgnButtonEventCR ev) override;
	//virtual     bool					_OnResetButton(DgnButtonEventCR ev) override;
	//virtual		StatusInt				_OnElementModify(EditElementHandleR eeh) override;


	bool		CreateBoundingbox(EditElementHandleR eeh, bvector<DPoint3d> const& points);
	bool		CreateElement(EditElementHandleR eeh, bvector<DPoint3d> points);
	bool		ValidateSelection(HitPathCP hitPath);

	virtual		void					_LocateOneElement(DgnButtonEventCR ev, bool newSearch) override;
	virtual		HitPathCP				_DoLocate(DgnButtonEventCR ev, bool newSearch, ComponentMode complexComponent) override;
	//virtual		EditElementHandleP		_BuildLocateAgenda(HitPathCP path, DgnButtonEventCP ev) override;
	virtual		void					_ModifyAgendaEntries() override;
	virtual		bool					_FilterAgendaEntries() override;
	virtual		void					_HiliteAgendaEntries(bool changed) override;
	//virtual		bool					_SetupForModify(DgnButtonEventCR ev, bool isDynamics) override;
	virtual		StatusInt				_ProcessAgenda(DgnButtonEventCR ev) override;
	virtual		bool					_OnModifyComplete(DgnButtonEventCR ev) override;
	virtual		void					_OnReinitialize() override;
	
};