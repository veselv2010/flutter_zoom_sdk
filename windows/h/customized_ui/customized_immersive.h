/**
 * @file meeting_immersive_interface.h
 * @brief ZOOM Custom Immersive Interface
 */
#ifndef _MEETING_IMMERSIVE_INTERFACE_H_
#define _MEETING_IMMERSIVE_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/**
 * @brief Enumeration of immersive template type.
 * View more detailed structural descriptions.
 */
enum CustomImmersiveTemplateType
{
	/** The default template. */
	CustomImmersiveTemplateType_Default,	
	/** A template using a custom image. This kind of template can only be used after CustomImmersiveTemplateType_MyVideo template is ready. */
	CustomImmersiveTemplateType_CustomImage,
	/** The my video template. */
	CustomImmersiveTemplateType_MyVideo		
};

/**
 * @brief Information of seat placement in a template.
 */
struct SeatPlacementInfo
{
	/** The seat ID. */
	unsigned int seat_id;	
	/** The seat position. */
	RECT position;			

	SeatPlacementInfo()
	{
		seat_id = 0;
		position = { 0 };
	}
};

/**
 * @brief Layout data of immersive.
 */
struct CustomImmersiveLayoutData
{
	/** Whether this user is in seat. */
	bool is_seat_free;		
	/** The seat ID. */
	unsigned int seat_id;	
	/** The user ID. */
	unsigned int user_id;	
	/** The seat z order. Higher numbers are displayed on top of lower ones. */
	unsigned int z_order;	
	/** The seat position. */
	RECT position;			

	CustomImmersiveLayoutData()
	{
		is_seat_free = false;
		seat_id = 0;
		user_id = 0;
		z_order = 0;
		position = { 0 };
	}
};

/**
 * @class ICustomImmersiveTemplateData
 * @brief Immersive template data object interface.
 */
class ICustomImmersiveTemplateData
{
public:
	/**
	 * @brief Gets the size of this template.
	 * @return If the function succeeds, it returns the size of this template.
	 */
	virtual SIZE getCanvasSize() = 0;
	
	/**
	 * @brief Gets the list of template seats.
	 * @return The list of template seats. ZERO(0) indicates that there are no seats in the template.
	 */
	virtual IList<SeatPlacementInfo>* getSeatList() = 0;

	virtual ~ICustomImmersiveTemplateData() {}
};

/**
 * @class ICustomImmersiveTemplate
 * @brief Immersive template object interface.
 */
class ICustomImmersiveTemplate
{
public:
	/**
	 * @brief Gets the name of this template.
	 * @return If the function succeeds, it returns the name of this template. Otherwise, this function fails and returns nullptr.
	 */
	virtual const zchar_t* getTemplateName() = 0;
	
	/**
	 * @brief Gets the bitmap of the thumbnail.
	 * @return If the function succeeds, it returns the bitmap of this thumbnail. Otherwise, this function fails and returns nullptr.
	 */
	virtual const void* getThumbnailBitmap() = 0;
	
	/**
	 * @brief Gets the capacity of this template.
	 * @return If the function succeeds, it returns the seat capacity of this template. 
	 */
	virtual unsigned int getCapacity() = 0;
	
	/**
	 * @brief Determines if this template is ready. 
	 * @return true indicates that this template is ready.
	 */
	virtual bool isTemplateReady() = 0;
	
	/**
	 * @brief Determines whether this template supports a free seat. 
	 * @return true indicates that the user can be put to free seat in this template.
	 */
	virtual bool isSupportFreeSeat() = 0;
	
	/**
	 * @brief Gets the type of this template.
	 * @return If the function succeeds, it returns the type of this template.
	 */
	virtual CustomImmersiveTemplateType getType() = 0;
	
	/**
	 * @brief Gets the template data of immersive when in share mode
	 * @return If the function succeeds, it returns the template data of immersive when in share mode. Otherwise, this function fails and returns nullptr.
	 */
	virtual ICustomImmersiveTemplateData* getShareTemplateData() = 0;
	
	/**
	 * @brief Gets the template data of immersive when in default mode
	 * @return If the function succeeds, it returns the template data of immersive when in default mode. Otherwise, this function fails and returns nullptr.
	 */
	virtual ICustomImmersiveTemplateData* getVideoTemplateData() = 0;

	virtual ~ICustomImmersiveTemplate() {}
};

/**
 * @class ICustomImmersivePreLayoutHelper
 * @brief Immersive pre-layout helper interface to update immersive view layout at once. 
 */
class ICustomImmersivePreLayoutHelper
{
public:
	/**
	 * @brief Add a user to the pre-layout with a seat ID.
	 * @param userID The user ID.
	 * @param seatID The seat ID.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, the function returns an error.
	 */
	virtual SDKError addUser(unsigned int userID, unsigned int seatID) = 0;
	
	/**
	 * @brief Add a user to the pre-layout with a position.
	 * @param userID The user ID.
	 * @param pos The position.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, the function returns an error.
	 */
	virtual SDKError addUser(unsigned int userID, RECT pos) = 0;
	
	/**
	 * @brief Remove a user from the pre-layout.
	 * @param userID The user ID.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, the function returns an error.
	 */
	virtual SDKError removeUser(unsigned int userID) = 0;
	
	/**
	 * @brief Remove all users from the pre-layout.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, the function returns an error.
	 */
	virtual SDKError removeAllUsers() = 0;
	
	/**
	 * @brief Gets the pre-layout data.
	 * @return The pre-layout data. ZERO(0) indicates that there are no user in the pre-layout.
	 */
	virtual IList<CustomImmersiveLayoutData>* getPreLayoutData() = 0;
	
	/**
	 * @brief Commit pre-layout data to immersive view. This only works just after the immersive view starts. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS and the user list will be cleaned up. Otherwise, the function returns an error.
	 */
	virtual SDKError commit() = 0;

	virtual ~ICustomImmersivePreLayoutHelper() {}
};

/**
 * @class ICustomImmersiveCtrlEvent
 * @brief Immersive controller object interface.
 */
class ICustomImmersiveCtrlEvent
{
public:
	/**
	 * @brief Callback event that the immersive view was enabled/disabled.
	 * @param bOn The status of immersive status.
	 */
	virtual void onImmersiveStatusChanged(bool bOn) = 0;
	
	/**
	 * @brief Callback event that the selected immersive template changed.
	 * @param immersiveTemplate The new template.
	 */
	virtual void onSelectedImmersiveTemplateChanged(ICustomImmersiveTemplate* immersiveTemplate) = 0;
	
	/**
	 * @brief Callback event that the immersive seat layout changed.
	 * @param seatList The new seat layout.
	 */
	virtual void onImmersiveSeatLayoutUpdated(IList<CustomImmersiveLayoutData>* seatList) = 0;
	
	/**
	 * @brief Callback event for the immersive template download process.
	 * @param immersiveTemplate The new template.
	 * @param progress The process.
	 */
	virtual void onTemplateDownloadProgress(ICustomImmersiveTemplate* immersiveTemplate, unsigned int progress) = 0;
	
	/**
	 * @brief Callback event for the immersive template download end.
	 * @param immersiveTemplate The new template.
	 * @param bSuccess The download result.
	 */
	virtual void onTemplateDownloadEnded(ICustomImmersiveTemplate* immersiveTemplate, bool bSuccess) = 0;
	
	/**
	 * @brief Callback event that template thumbnails download end.
	 * @param bSuccess The download result.
	 */
	virtual void onTemplateThumbnailsDownloadEnded(bool bSuccess) = 0;

	virtual ~ICustomImmersiveCtrlEvent() {}
};

/**
 * @class ICustomImmersiveController
 * @brief Meeting immersive controller interface. For more details on this feature, see https://support.zoom.us/hc/en-us/articles/360060220511-Immersive-View
 */
class ICustomImmersiveController
{
public:
	/**
	 * @brief Sets immersive object callback event handler.
	 * @param pEvent A pointer to the ICustomImmersiveCtrlEvent that receives the immersive object events.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetEvent(ICustomImmersiveCtrlEvent* pEvent) = 0;
	
	/**
	 * @brief Determines if immersive is supported. 
	 * @param [out] bSupport true indicates support immersive. Otherwise, false. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError isSupportImmersive(bool& bSupport) = 0;
	
	/**
	 * @brief Determines if immersive view is active. 
	 * @param [out] bOn true indicates the immersive view is active. Otherwise, false. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError isImmersiveViewOn(bool& bOn) = 0;
	
	/**
	 * @brief Download the template thumbnails. See /link ICustomImmersiveCtrlEvent /endlink for updates on the download.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError downloadTemplateThumbnails() = 0;
	
	/**
	 * @brief Determines if the thumbnails are ready. 
	 * @param [out] bReady true indicates the immersive thumbnails is ready, false not. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError isTemplateThumbnailsReady(bool& bReady) = 0;
	
	/**
	 * @brief Gets the list of templates.
	 * @return The list of templates. ZERO(0) indicates that there are no templates.
	 */
	virtual IList<ICustomImmersiveTemplate*>* getTemplates() = 0;
	
	/**
	 * @brief Download complete template resource. 
	 * @param immersiveTemplate The template to be downloaded.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError downloadTemplate(ICustomImmersiveTemplate* immersiveTemplate) = 0;
	
	/**
	 * @brief Determines if the immersive template can be started. 
	 * @param immersiveTemplate The selected template in immersive view.
	 * @param [out] bCan true indicates the immersive can be started, false not. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError canStartImmersiveView(ICustomImmersiveTemplate* immersiveTemplate, bool& bCan) = 0;
	
	/**
	 * @brief Starts immersive view. 
	 * @param immersiveTemplate The selected template in immersive view.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 * @note For the host, it must be started after the immersive container is created.
	 */
	virtual SDKError startImmersiveView(ICustomImmersiveTemplate* immersiveTemplate) = 0;
	
	/**
	 * @brief Change template in immersive view. 
	 * @param immersiveTemplate The selected template in immersive view.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError changeTemplate(ICustomImmersiveTemplate* immersiveTemplate) = 0;
	
	/**
	 * @brief Exit immersive view. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError endImmersiveView() = 0;
	
	/**
	 * @brief Gets the current template. 
	 * @return If the function succeeds, it returns the current template. Otherwise, this function fails and returns nullptr.
	 */
	virtual ICustomImmersiveTemplate* getCurrentTemplate() = 0;
	
	/**
	 * @brief Determines if the user can be shown in immersive view. 
	 * @param userID The user ID.
	 * @param [out] bCan true indicates the user can be shown in immersive view, false if they cannot. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError canUserShowInImmersiveView(unsigned int userID, bool& bCan) = 0;
	
	/**
	 * @brief Gets the immersive pre-layout helper pointer. 
	 * @return If the function succeeds, it returns the immersive layout helper pointer. Otherwise, this function fails and returns nullptr.
	 */
	virtual ICustomImmersivePreLayoutHelper* getImmersivePreLayoutHelper() = 0;
	
	/**
	 * @brief Gets the immersive seat layout data.
	 * @return The immersive seat layout data. ZERO(0) indicates that there are no users in the immersive view.
	 */
	virtual IList<CustomImmersiveLayoutData>* getLayoutData() = 0;
	
	/**
	 * @brief Put the user in the seat. 
	 * @param userID The user ID.
	 * @param seatID The seat ID.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError assignUser(unsigned int userID, unsigned int seatID) = 0;
	
	/**
	 * @brief Put the user in the free seat. 
	 * @param userID The user ID.
	 * @param pos The position.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError putUserToFreeSeat(unsigned int userID, RECT pos) = 0;
	
	/**
	 * @brief Remove user from immersive view. 
	 * @param userID The user ID.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError removeUser(unsigned int userID) = 0;
	
	/**
	 * @brief Determines if the user is in immersive view. 
	 * @param userID The user ID.
	 * @param [out] bIn true indicates the user is in immersive view, false means that they are not. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError isUserInImmersiveView(unsigned int userID, bool& bIn) = 0;
	
	/**
	 * @brief Add a template based on a custom image. 
	 * @param filePath The image file path.
	 * @param [out] immersiveTemplate The object of custom template.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError addCustomImageTemplate(const zchar_t* filePath, ICustomImmersiveTemplate** immersiveTemplate) = 0;
	
	/**
	 * @brief Remove custom image template. 
	 * @param immersiveTemplate The custom image template that want to remove.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError removeCustomImageTemplate(ICustomImmersiveTemplate* immersiveTemplate) = 0;
	
	/**
	 * @brief Determines if displaying sharing contents in immersive mode.
	 * @param [out] bInShare true indicates displaying sharing contents in immersive mode, false means that they are not. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError isInImmersiveShareMode(bool& bInShare) = 0;
	
	/**
	 * @brief Update the share source ID to view share, only available for host.
	 * @param shareSourceID The sepecified share source ID.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError viewShare(unsigned int shareSourceID) = 0;
	
	/**
	 * @brief Query the share source ID when viewing share in immersive mode, only available for host.
	 * @param [out] shareSourceID The sepecified share source ID.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, failed.
	 */
	virtual SDKError getViewingShareSourceID(unsigned int& shareSourceID) = 0;

	virtual ~ICustomImmersiveController() {}
};



END_ZOOM_SDK_NAMESPACE
#endif