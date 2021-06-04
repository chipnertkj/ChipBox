// Audio engine classes
#pragma once
#include "HostData.h" // interface

/*

#include <public.sdk/source/vst/hosting/hostclasses.h>
#include <public.sdk/source/vst/hosting/eventlist.h>
#include <public.sdk/source/vst/hosting/pluginterfacesupport.h>
#include <public.sdk/source/vst/hosting/parameterchanges.h>

#include <pluginterfaces/base/funknown.h>
#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivstpluginterfacesupport.h>
#include <pluginterfaces/vst/ivstunits.h>
#include <pluginterfaces/gui/iplugview.h>



namespace host {
	namespace St = Steinberg;
	namespace Vst = Steinberg::Vst;
	

	// Attribute list used in IMessage and IStreamAttributes.
	// An attribute list associates values with a key.
	class HostAttribute;
	class AttributeList : public Vst::IAttributeList {
	public:
		AttributeList();
		~AttributeList();

		St::tresult PLUGIN_API setInt(AttrID aid, St::int64 value) SMTG_OVERRIDE;
		St::tresult PLUGIN_API getInt(AttrID aid, St::int64& value) SMTG_OVERRIDE;
		St::tresult PLUGIN_API setFloat(AttrID aid, double value) SMTG_OVERRIDE;
		St::tresult PLUGIN_API getFloat(AttrID aid, double& value) SMTG_OVERRIDE;
		St::tresult PLUGIN_API setString(AttrID aid, const Vst::TChar* string) SMTG_OVERRIDE;
		St::tresult PLUGIN_API getString(AttrID aid, Vst::TChar* string, St::uint32 sizeInBytes) SMTG_OVERRIDE;
		St::tresult PLUGIN_API setBinary(AttrID aid, const void* data, St::uint32 sizeInBytes) SMTG_OVERRIDE;
		St::tresult PLUGIN_API getBinary(AttrID aid, const void*& data, St::uint32& sizeInBytes) SMTG_OVERRIDE;

		DECLARE_FUNKNOWN_METHODS
	protected:
		void removeAttrID(AttrID aid);
		std::map<St::String, HostAttribute*> list;
	};

	// Host callback interface for an edit controller.
	// Allow transfer of parameter editing to component (processor) via host and support automation.
	// Cause the host to react on configuration changes (restartComponent).
	class ComponentHandler : public Vst::IComponentHandler {
	public:
		ComponentHandler();
		~ComponentHandler();
		// called before performEdit
		St::tresult PLUGIN_API beginEdit(Vst::ParamID id) override;
		// on edit
		St::tresult PLUGIN_API performEdit(Vst::ParamID id, Vst::ParamValue valueNormalized) override;
		// called after performEdit
		St::tresult PLUGIN_API endEdit(Vst::ParamID id) override;
		// ?
		St::tresult PLUGIN_API restartComponent(St::int32 flags) override;

		DECLARE_FUNKNOWN_METHODS
	};

	// List of events to process.
	class EventList : public Vst::EventList {};

	// Host callback interface, used with IUnitInfo.
	// Retrieve via queryInterface from IComponentHandler.
	class UnitHandler : public Vst::IUnitHandler {
	public:
		UnitHandler();
		~UnitHandler();
		// host module selected through plug gui
		St::tresult PLUGIN_API notifyUnitSelection(Vst::UnitID unitId) override;
		// plug controller changed a program list
		St::tresult PLUGIN_API notifyProgramListChange(Vst::ProgramListID listId, St::int32 programIndex) override;

		DECLARE_FUNKNOWN_METHODS
	};

	// Host callback interface for an edit controller: Vst::IPlugInterfaceSupport.
	// Allows a plug-in to ask the host if a given plug-in interface is supported/used by the host.
	// It is implemented by the hostContext given when the component is initialized.
	class PlugInterfaceSupport : public St::FObject, public Vst::IPlugInterfaceSupport {
	public:
		PlugInterfaceSupport();

		//--- IPlugInterfaceSupport ---------
		St::tresult PLUGIN_API isPlugInterfaceSupported(const St::TUID _iid) SMTG_OVERRIDE;

		void addPlugInterfaceSupported(const St::TUID _iid);
		bool removePlugInterfaceSupported(const St::TUID _iid);

		OBJ_METHODS(PlugInterfaceSupport, FObject)
		REFCOUNT_METHODS(FObject)
		DEFINE_INTERFACES
		DEF_INTERFACE(IPlugInterfaceSupport)
		END_DEFINE_INTERFACES(FObject)

	private:
		std::vector<St::FUID> mFUIDArray;
	};

	// Basic host callback interface.
	// Basic VST host application interface.
	// Cloned from hostclasses.cpp
	class HostApplication : public Vst::IHostApplication {
	public:
		HostApplication();
		~HostApplication();
		// get host name as a 128 TChar long string
		St::tresult PLUGIN_API getName(Vst::String128 name);
		// create host objects (e.g. Vst::IMessage)
		St::tresult PLUGIN_API createInstance(St::TUID cid, St::TUID _iid, void** obj);
		// PlugInterfaceSupport getter
		Vst::PlugInterfaceSupport* PLUGIN_API getPlugInterfaceSupport() const { return mPlugInterfaceSupport; }

		DECLARE_FUNKNOWN_METHODS
	protected:
		St::IPtr<Vst::PlugInterfaceSupport> mPlugInterfaceSupport;
	};

	// Private plug-in message.
	// Messages are sent from a VST controller component to a VST editor component and vice versa.
	class Message : public Vst::IMessage {
	public:
		Message();
		~Message();

		const char* PLUGIN_API getMessageID() SMTG_OVERRIDE;
		void PLUGIN_API setMessageID(const char* messageID) SMTG_OVERRIDE;
		Vst::IAttributeList* PLUGIN_API getAttributes() SMTG_OVERRIDE;

		DECLARE_FUNKNOWN_METHODS
	protected:
		char* messageId;
		AttributeList* attributeList;
	};

	// Queue of changes for a specific parameter.
	// A more detailed description: https://steinbergmedia.github.io/vst3_doc/vstinterfaces/classSteinberg_1_1Vst_1_1IParamValueQueue.html
	class ParamValueQueue : public Vst::ParameterValueQueue {};

	// All parameter changes of a processing block.
	// This interface is used to transmit any changes to be applied to parameters in the current processing block.
	// A change can be caused by GUI interaction as well as automation.
	// They are transmitted as a list of queues(IParamValueQueue) containing only queues for parameters that actually did change.
	class ParameterChanges : public Vst::ParameterChanges {};

	// Callback interface passed to IPlugView.
	// Enables a plug - in to resize the view and cause the host to resize the window.
	class PlugFrame : public St::IPlugFrame {
	public:
		PlugFrame();
		~PlugFrame();
		// called on plugin gui view resize (that's when you're supposed to resize its window)
		St::tresult resizeView(St::IPlugView* view, St::ViewRect* newSize);
			
		DECLARE_FUNKNOWN_METHODS
	};
}
*/