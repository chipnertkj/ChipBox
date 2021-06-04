// Audio engine classes
#include "HostClasses.h" // interface
#include <map>

/*
#pragma warning(disable : 26812)


namespace host {
	namespace St = Steinberg;
	namespace Vst = Steinberg::Vst;

	//---------------------------------------------------------
	// AttributeList
	class HostAttribute
	{
	public:
		enum Type
		{
			kInteger,
			kFloat,
			kString,
			kBinary
		};

		HostAttribute(St::int64 value) : size(0), type(kInteger) { v.intValue = value; }
		HostAttribute(double value) : size(0), type(kFloat) { v.floatValue = value; }
		HostAttribute(const Vst::TChar* value, St::uint32 sizeInCodeUnit) : size(sizeInCodeUnit), type(kString)
		{
			v.stringValue = new Vst::TChar[sizeInCodeUnit];
			memcpy(v.stringValue, value, sizeInCodeUnit * sizeof(Vst::TChar));
		}
		HostAttribute(const void* value, St::uint32 sizeInBytes) : size(sizeInBytes), type(kBinary)
		{
			v.binaryValue = new char[sizeInBytes];
			memcpy(v.binaryValue, value, sizeInBytes);
		}
		~HostAttribute()
		{
			if (size)
				delete[] v.binaryValue;
		}

		St::int64 intValue() const { return v.intValue; }
		double floatValue() const { return v.floatValue; }
		const Vst::TChar* stringValue(St::uint32& sizeInCodeUnit)
		{
			sizeInCodeUnit = size;
			return v.stringValue;
		}
		const void* binaryValue(St::uint32& sizeInBytes)
		{
			sizeInBytes = size;
			return v.binaryValue;
		}

		Type getType() const { return type; }

	protected:
		union v
		{
			St::int64 intValue;
			double floatValue;
			Vst::TChar* stringValue;
			char* binaryValue;
		} v;
		St::uint32 size;
		Type type;
	};

	IMPLEMENT_FUNKNOWN_METHODS(AttributeList, Vst::IAttributeList, Vst::IAttributeList::iid)
	AttributeList::AttributeList()
	{
		FUNKNOWN_CTOR
	}
	AttributeList::~AttributeList()
	{
		auto it = list.rbegin();
		while (it != list.rend())
		{
			delete it->second;
			it++;
		}
		FUNKNOWN_DTOR
	}
	void AttributeList::removeAttrID(AttrID aid)
	{
		auto it = list.find(aid);
		if (it != list.end())
		{
			delete it->second;
			list.erase(it);
		}
	}
	St::tresult PLUGIN_API AttributeList::setInt(AttrID aid, St::int64 value)
	{
		removeAttrID(aid);
		list[aid] = new HostAttribute(value);
		return St::kResultTrue;
	}
	St::tresult PLUGIN_API AttributeList::getInt(AttrID aid, St::int64& value)
	{
		auto it = list.find(aid);
		if (it != list.end() && it->second)
		{
			value = it->second->intValue();
			return St::kResultTrue;
		}
		return St::kResultFalse;
	}
	St::tresult PLUGIN_API AttributeList::setFloat(AttrID aid, double value)
	{
		removeAttrID(aid);
		list[aid] = new HostAttribute(value);
		return St::kResultTrue;
	}
	St::tresult PLUGIN_API AttributeList::getFloat(AttrID aid, double& value)
	{
		auto it = list.find(aid);
		if (it != list.end() && it->second)
		{
			value = it->second->floatValue();
			return St::kResultTrue;
		}
		return St::kResultFalse;
	}
	St::tresult PLUGIN_API AttributeList::setString(AttrID aid, const Vst::TChar* string)
	{
		removeAttrID(aid);
		// + 1 for the null-terminate
		list[aid] = new HostAttribute(string, St::String(string).length() + 1);
		return St::kResultTrue;
	}
	St::tresult PLUGIN_API AttributeList::getString(AttrID aid, Vst::TChar* string, St::uint32 sizeInBytes)
	{
		auto it = list.find(aid);
		if (it != list.end() && it->second)
		{
			St::uint32 sizeInCodeUnit = 0;
			const Vst::TChar* _string = it->second->stringValue(sizeInCodeUnit);
			memcpy(string, _string, std::min<St::uint32>(sizeInCodeUnit * sizeof(Vst::TChar), sizeInBytes));
			return St::kResultTrue;
		}
		return St::kResultFalse;
	}
	St::tresult PLUGIN_API AttributeList::setBinary(AttrID aid, const void* data, St::uint32 sizeInBytes)
	{
		removeAttrID(aid);
		list[aid] = new HostAttribute(data, sizeInBytes);
		return St::kResultTrue;
	}
	St::tresult PLUGIN_API AttributeList::getBinary(AttrID aid, const void*& data, St::uint32& sizeInBytes)
	{
		auto it = list.find(aid);
		if (it != list.end() && it->second)
		{
			data = it->second->binaryValue(sizeInBytes);
			return St::kResultTrue;
		}
		sizeInBytes = 0;
		return St::kResultFalse;
	}

	//---------------------------------------------------------
	// ComponentHandler
	IMPLEMENT_FUNKNOWN_METHODS(ComponentHandler, Vst::IComponentHandler, Vst::IComponentHandler::iid)
	ComponentHandler::ComponentHandler() {
		FUNKNOWN_CTOR
	}
	ComponentHandler::~ComponentHandler() {
		FUNKNOWN_DTOR
	}
	St::tresult PLUGIN_API ComponentHandler::beginEdit(Vst::ParamID id) {

		return St::kResultOk;
	}
	St::tresult PLUGIN_API ComponentHandler::performEdit(Vst::ParamID id, Vst::ParamValue valueNormalized) {

		return St::kResultOk;
	}
	St::tresult PLUGIN_API ComponentHandler::endEdit(Vst::ParamID id) {

		return St::kResultOk;
	}
	St::tresult PLUGIN_API ComponentHandler::restartComponent(St::int32 flags) {

		return St::kResultOk;
	}
	// TODO: (FEATURE) implement ComponentHandler->gui interactions

	//---------------------------------------------------------
	// UnitHandler
	IMPLEMENT_FUNKNOWN_METHODS(UnitHandler, Vst::IUnitHandler, Vst::IUnitHandler::iid)
	UnitHandler::UnitHandler() {
		FUNKNOWN_CTOR
	}
	UnitHandler::~UnitHandler() {
		FUNKNOWN_DTOR
	}
	St::tresult PLUGIN_API UnitHandler::notifyUnitSelection(Vst::UnitID unitId) {

		return St::kResultOk;
	}
	St::tresult PLUGIN_API UnitHandler::notifyProgramListChange(Vst::ProgramListID listId, St::int32 programIndex) {

		return St::kResultOk;
	}
	// TODO: (FEATURE) implement UnitHandler->gui interactions

	//---------------------------------------------------------
	// PlugInterfaceSupport
	PlugInterfaceSupport::PlugInterfaceSupport() {
		// add minimum set

		//---VST 3.0.0--------------------------------
		addPlugInterfaceSupported(Vst::IComponent::iid);
		addPlugInterfaceSupported(Vst::IAudioProcessor::iid);
		addPlugInterfaceSupported(Vst::IEditController::iid);
		addPlugInterfaceSupported(Vst::IConnectionPoint::iid);

		addPlugInterfaceSupported(Vst::IUnitInfo::iid);
		addPlugInterfaceSupported(Vst::IUnitData::iid);
		addPlugInterfaceSupported(Vst::IProgramListData::iid);

		//---VST 3.0.1--------------------------------
		//addPlugInterfaceSupported(IMidiMapping::iid);

		//---VST 3.1----------------------------------
		//addPlugInterfaceSupported(IEditController2::iid);

		
		//---VST 3.0.2--------------------------------
		//addPlugInterfaceSupported (IParameterFinder::iid);

		//---VST 3.1----------------------------------
		//addPlugInterfaceSupported (IAudioPresentationLatency::iid);

		//---VST 3.5----------------------------------
		//addPlugInterfaceSupported (IKeyswitchController::iid);
		//addPlugInterfaceSupported (IContextMenuTarget::iid);
		//addPlugInterfaceSupported (IEditControllerHostEditing::iid);
		//addPlugInterfaceSupported (IXmlRepresentationController::iid);
		//addPlugInterfaceSupported (INoteExpressionController::iid);

		//---VST 3.6.5--------------------------------
		//addPlugInterfaceSupported (ChannelContext::IInfoListener::iid);
		//addPlugInterfaceSupported (IPrefetchableSupport::iid);
		//addPlugInterfaceSupported (IAutomationState::iid);

		//---VST 3.6.11--------------------------------
		//addPlugInterfaceSupported (INoteExpressionPhysicalUIMapping::iid);

		//---VST 3.6.12--------------------------------
		//addPlugInterfaceSupported (IMidiLearn::iid);

		//---VST 3.7-----------------------------------
		//addPlugInterfaceSupported (IProcessContextRequirements::iid);
		//addPlugInterfaceSupported (IParameterFunctionName::iid);
		//addPlugInterfaceSupported (IProgress::iid);
		
	}

	St::tresult PLUGIN_API PlugInterfaceSupport::isPlugInterfaceSupported(const St::TUID _iid) {
		auto uid = St::FUID::fromTUID(_iid);
		if (std::find(mFUIDArray.begin(), mFUIDArray.end(), uid) != mFUIDArray.end())
			return St::kResultTrue;
		return St::kResultFalse;
	}

	void PlugInterfaceSupport::addPlugInterfaceSupported(const St::TUID _iid)
	{
		mFUIDArray.push_back(St::FUID::fromTUID(_iid));
	}

	bool PlugInterfaceSupport::removePlugInterfaceSupported(const St::TUID _iid)
	{
		return std::remove(mFUIDArray.begin(), mFUIDArray.end(), St::FUID::fromTUID(_iid)) !=
			mFUIDArray.end();
	}

	//---------------------------------------------------------
	// HostApplication
	HostApplication::HostApplication() {
		FUNKNOWN_CTOR
		mPlugInterfaceSupport = St::owned(NEW Vst::PlugInterfaceSupport);
	}
	HostApplication::~HostApplication() {
		FUNKNOWN_DTOR
	}
	St::tresult PLUGIN_API HostApplication::getName(Vst::String128 name) {
		St::String str("ChipBox");
		str.copyTo16(name, 0, 127);
		return St::kResultTrue;
	}
	St::tresult PLUGIN_API HostApplication::createInstance(St::TUID cid, St::TUID _iid, void** obj) {
		St::FUID classID(St::FUID::fromTUID(cid));
		St::FUID interfaceID(St::FUID::fromTUID(_iid));
		if (classID == Vst::IMessage::iid && interfaceID == Vst::IMessage::iid)
		{
			*obj = new Message;
			return St::kResultTrue;
		}
		else if (classID == Vst::IAttributeList::iid && interfaceID == Vst::IAttributeList::iid)
		{
			*obj = new AttributeList;
			return St::kResultTrue;
		}
		*obj = nullptr;
		return St::kResultFalse;
	}
	St::tresult PLUGIN_API HostApplication::queryInterface(const char* _iid, void** obj) {
		QUERY_INTERFACE(_iid, obj, FUnknown::iid, IHostApplication)
			QUERY_INTERFACE(_iid, obj, IHostApplication::iid, IHostApplication)

			if (mPlugInterfaceSupport && mPlugInterfaceSupport->queryInterface(iid, obj) == St::kResultTrue)
				return St::kResultOk;

		*obj = nullptr;
		return St::kResultFalse;
	}
	St::uint32 PLUGIN_API HostApplication::addRef() {
		return 1;
	}
	St::uint32 PLUGIN_API HostApplication::release() {
		return 1;
	}

	//---------------------------------------------------------
	// Message
	IMPLEMENT_FUNKNOWN_METHODS(Message, Vst::IMessage, Vst::IMessage::iid)
		Message::Message() : messageId(nullptr), attributeList(nullptr) {
		FUNKNOWN_CTOR
	}
	Message::~Message() {
		setMessageID(nullptr);
		if (attributeList)
			attributeList->release();
		FUNKNOWN_DTOR
	}
	const char* PLUGIN_API Message::getMessageID() {
		return messageId;
	}
	void PLUGIN_API Message::setMessageID(const char* mid) {
		if (messageId)
			delete[] messageId;
		messageId = nullptr;
		if (mid)
		{
			size_t len = strlen(mid) + 1;
			messageId = new char[len];
			strcpy(messageId, mid);
		}
	}
	Vst::IAttributeList* PLUGIN_API Message::getAttributes() {
		if (!attributeList)
			attributeList = new AttributeList;
		return attributeList;
	}

	//---------------------------------------------------------
	// PlugFrame
	IMPLEMENT_FUNKNOWN_METHODS(PlugFrame, St::IPlugFrame, St::IPlugFrame::iid)
	PlugFrame::PlugFrame() {
		FUNKNOWN_CTOR
	}
	PlugFrame::~PlugFrame() {
		FUNKNOWN_DTOR
	}
	St::tresult PlugFrame::resizeView(St::IPlugView* view, St::ViewRect* newSize) {
		return St::kResultOk;
	}
}
*/