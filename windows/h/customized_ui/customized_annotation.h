/**
 * @file customized_annotation.h
 * @brief ZOOM Custom Annotation Interface. 
 */
#ifndef _ZOOM_CUSTOMIZED_ANNOTATION_H_
#define _ZOOM_CUSTOMIZED_ANNOTATION_H_
#include "zoom_sdk_def.h"
#include "meeting_service_components\meeting_annotation_interface.h"

BEGIN_ZOOM_SDK_NAMESPACE
class ICustomizedShareRender;

/**
 * @brief Enumeration of custom annotation toolbar status. 
 * Here are more detailed structural descriptions..
 */ 
enum CustomizedShareAnnotationStatus
{
	/** The toolbar has been created. */
	CS_ANNO_READY_TO_USE,
	/** The toolbar will be destroyed. */
	CS_ANNO_CLOSE, 
};

/**
 * @brief Enumeration of annotation save type.
 */
enum SDKAnnoSaveType
{
	SDK_ANNO_SAVE_NONE = 0,
	SDK_ANNO_SAVE_PNG,
	SDK_ANNO_SAVE_PDF,
	SDK_ANNO_SAVE_PNG_MEMORY,
	SDK_ANNO_SAVE_PDF_MEMORY,
	SDK_ANNO_SAVE_BITMAP_MEMORY,
};

/**
 * @class ICustomizedAnnotationObjEvent
 * @brief Annotation object callback event.
 */                                    
class ICustomizedAnnotationObjEvent
{
public:
	/**
	 * @brief Callback event that the annotation tool changes.
	 * @param type_ The type of annotation tool.
	 */
	virtual void onAnnotationObjToolChange(AnnotationToolType type_) = 0;
};

/**
 * @class ICustomizedAnnotationObj
 * @brief Annotation object interface.
 */
class ICustomizedAnnotationObj
{
public:
	/**
	 * @brief Sets annotation object callback event handler.
	 * @param event_ A pointer to the ICustomizedAnnotationObjEvent that receives annotation object event. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetEvent(ICustomizedAnnotationObjEvent* event_) = 0;
	
	/**
	 * @brief Determines if it is enabled to clear annotations by the specified way. 
	 * @param type Specify the annotation clear type.
	 * @return If the user owns the authority, the return value is SDKERR_SUCCESS. Otherwise not.
	 */
	virtual SDKError CanClear(AnnotationClearType type) = 0;
	
	/**
	 * @brief Clear the annotation with the specified type.
	 * @param type Specify the type to clear annotation.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */

	virtual SDKError Clear(AnnotationClearType type) = 0;
	
	/**
	 * @brief Sets the tool to annotate.
	 * @param type Specify the type of the annotation tool.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetTool(AnnotationToolType type) = 0;
	
	/**
	 * @brief Sets the color to annotate.
	 * @param color Specify the color to annotate, in RGB format.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetColor(unsigned long color) = 0;
	
	/**
	 * @brief Sets the value of line width of annotation tool.
	 * @param lineWidth The line width of annotation tool.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetLineWidth(long lineWidth) = 0;
	
	/**
	 * @brief Gets the color of current annotation tool.
	 * @param [out] color The color to annotate in RGB format.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError GetCurColor(unsigned long& color) = 0;
	
	/**
	 * @brief Gets the value of line width of the current annotation tool.
	 * @param [out] lineWidth The width of the current annotation tool. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError GetCurLineWidth(long& lineWidth) = 0;
	
	/**
	 * @brief Gets the type of the current annotation tool.
	 * @param type Specify the type of the annotation tool.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError GetCurTool(AnnotationToolType& type) = 0;
	
	/**
	 * @brief Undo the last annotation.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError Undo() = 0;
	
	/**
	 * @brief Redo the annotation that was undone. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError Redo() = 0;
	
	/**
	 * @brief Determines if it is enabled to save the screenshot.
	 * @return If the user owns the authority, the return value is SDKERR_SUCCESS. Otherwise not.
	 */
	virtual SDKError CanSaveSnapshot() = 0;
	
	/**
	 * @brief Save the screenshot in the specified path.
	 * @param path Specify the path to store the screenshot. If the specified path is wrong, the SDKERR_INVALID_PARAMETER will be returned. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SaveSnapshot(const zchar_t* path, SDKAnnoSaveType nType) = 0;
	
	/**
	 * @brief Determines if can do annotate.
	 * @param [out] bCan, true indicates can do annotate, false can not. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError CanDoAnnotation(bool& bCan) = 0;
	
	/**
	 * @brief Determines whether current meeting supports annotation feature.
	 * @return true not support, false support.
	 */
	virtual bool IsAnnotationDisable() = 0;
	
	/**
	 * @brief Disallow/allow participants to annotate when viewing the sharing content.
	 * @param bDisable true indicates disabled, false not.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError DisableViewerAnnotation(bool bDisable) = 0;
	
	/**
	 * @brief Determines if it is able to disallow viewer to annotate. 
	 * @param [out] bCan true indicates able, false not. It validates only when the return value is SDKERR_SUCCESS. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError CanDisableViewerAnnotation(bool& bCan) = 0;
	
	/**
	 * @brief Determines if viewer's privilege of annotation is disabled.
	 * @return true if viewer's privilege of annotation is disabled. Otherwise, false.
	 */
	virtual bool IsViewerAnnotationDisabled() = 0;

	virtual ~ICustomizedAnnotationObj(){};
};

/**
 * @class ICustomizedAnnotationControllerEvent
 * @brief Annotation controller callback event.
 */
class ICustomizedAnnotationControllerEvent
{
public:
	/**
	 * @brief Callback of destroying the specified annotation object.
	 * @param obj_ Specify the annotation object to be destroyed. Once destroyed, it can no longer be used.
	 */
	virtual void onCustomizedAnnotationObjDestroyed(ICustomizedAnnotationObj* obj_) = 0;
	
	/**
	 * @brief Callback event when the annotation status changes.
	 * @param share_render_ The annotate status of share_render changes.
	 * @param status_ The changed status.
	 */
	virtual void onSharingShareAnnotationStatusChanged(ICustomizedShareRender* share_render_, CustomizedShareAnnotationStatus status_) = 0;
};

/**
 * @class ICustomizedAnnotationController
 * @brief Annotation controller interface.
 */
class ICustomizedAnnotationController
{
public:
	/**
	 * @brief Sets annotation controller callback event handler.
	 * @param event_ A pointer to the ICustomizedAnnotationControllerEvent that receives annotation controller event. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetEvent(ICustomizedAnnotationControllerEvent* event_) = 0;
	
	/**
	 * @brief An instance created on the specified render which is an object of \link ICustomizedShareRender \endlink created on the sharing window.
	 * @param view_share_render Specify the render to receive the shared content. The sharer should set the value to nullptr.
	 * @param pp_obj A pointer to the ICustomizedAnnotationObj*.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS, and ppMeetingService is not nullptr. Otherwise, this function returns an error.
	 * @note It is suggested to call this function if the value of status_ is CS_ANNO_READY_TO_USE when you receive the \link ICustomizedAnnotationControllerEvent::onSharingShareAnnotationStatusChanged \endlink .
	 */
	virtual SDKError CreateAnnoObj(ICustomizedShareRender* view_share_render, ICustomizedAnnotationObj** pp_obj) = 0;
	
	/**
	 * @brief Destroy the specified annotation object.
	 * @param anno_obj Specify the annotation tool to be destroyed.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError DestroyAnnoObj(ICustomizedAnnotationObj* anno_obj) = 0;
	virtual ~ICustomizedAnnotationController(){};

}; 
END_ZOOM_SDK_NAMESPACE
#endif