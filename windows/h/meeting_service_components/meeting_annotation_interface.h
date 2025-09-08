/**
 * @file meeting_annotation_interface.h
 * @brief Annotation of Meeting Service Interface.
 * 
 */
#ifndef _MEETING_Annotation_INTERFACE_H_
#define _MEETING_Annotation_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE
class ICustomizedShareRender;
/** 
 * @brief Types of annotation tool.
 * Here are more detailed structural descriptions.
 */
enum AnnotationToolType
{
	/** Switch to mouse cursor. */
	ANNOTOOL_NONE_DRAWING,
	/** Pen. */
	ANNOTOOL_PEN,
	/** Highlighter. */
	ANNOTOOL_HIGHLIGHTER,
	/** A straight line changes automatically in pace with the mouse cursor. */
	ANNOTOOL_AUTO_LINE,
	/** A rectangle changes automatically in pace with the mouse cursor. */
	ANNOTOOL_AUTO_RECTANGLE,
	/** An ellipse changes automatically in pace with the mouse cursor. */
	ANNOTOOL_AUTO_ELLIPSE,
	/** An arrow changes automatically in pace with the mouse cursor. */
	ANNOTOOL_AUTO_ARROW,
	/** A filled rectangle. */
	ANNOTOOL_AUTO_RECTANGLE_FILL,
	/** A filled ellipse. */
	ANNOTOOL_AUTO_ELLIPSE_FILL,
	/** Laser pointer. */
	ANNOTOOL_SPOTLIGHT,
	/** An arrow showing the name of whom click on the sharing content. */
	ANNOTOOL_ARROW,
	/** An eraser. */
	ANNOTOOL_ERASER,
	/** Insert a textbox in order to input letters. */
	ANNOTOOL_TEXTBOX, 
	/** Select the annotations. */
	ANNOTOOL_PICKER, 
	/** A fair rectangle changes automatically in pace with the mouse cursor. */
	ANNOTOOL_AUTO_RECTANGLE_SEMI_FILL, 
	/** A fair ellipse changes automatically in pace with the mouse cursor. */
	ANNOTOOL_AUTO_ELLIPSE_SEMI_FILL, 
	/** A line with double-arrow.  */
	ANNOTOOL_AUTO_DOUBLE_ARROW, 
	/** An unfilled rhombus. */
	ANNOTOOL_AUTO_DIAMOND, 
	/** A fixed-size arrow for marking. */
	ANNOTOOL_AUTO_STAMP_ARROW, 
	/** A sign marking that something is correct. */
	ANNOTOOL_AUTO_STAMP_CHECK, 
	/** A sign marking that something is wrong. */
	ANNOTOOL_AUTO_STAMP_X, 
	/** A star for marking. */
	ANNOTOOL_AUTO_STAMP_STAR, 
	/** A heart for marking. */
	ANNOTOOL_AUTO_STAMP_HEART, 
	/** A sign for interrogation. */
	ANNOTOOL_AUTO_STAMP_QM, 
};
/** 
 * @brief Clear types of annotation tools.
 * Here are more detailed structural descriptions.
 */
enum AnnotationClearType
{
	/** Clear all annotations. Hosts, managers and shared meeting owners can use. */
	ANNOCLEAR_ALL,
	/** Clear only your own annotations. Everyone can use. */
	ANNOCLEAR_SELF,
	/** Clear only the others' annotations. Only shared meeting owners can use. */
	ANNOCLEAR_OTHER,
};

/** 
 * @class IMeetingAnnotationSupportEvent
 * @brief Callback interface that viewer's annotation status changes.
 */
class IMeetingAnnotationSupportEvent
{
public:
	/** 
	 * @brief The SDK will trigger this callback if the presenter enable/disable PARTICIPANTS ANNOTATION.
	 */
	virtual void onSupportAnnotationStatus(unsigned int nShareSourceID, bool bSupportAnnotation) = 0;

	virtual ~IMeetingAnnotationSupportEvent() {}
};

class ICustomizedAnnotationController;
/** 
 * @class IAnnotationController
 * @brief Meeting annotation tool interface.
 */
class IAnnotationController
{
public:
	/**
	 * @brief Set the callback that annotation status changes. 
	 * @param pEvent An object pointer to the the IMeetingAnnotationSupportEvent that receives the annotation status changing callback event. 
	 * @return If the user owns the authority, the return value is SDKErr_Success. Otherwise not. To get extended error information, see \link SDKError \endlink enum.
	 */
	virtual SDKError SetEvent(IMeetingAnnotationSupportEvent* pEvent) = 0;

	/**
	 * @brief Determine whether current meeting supports annotation feature.
	 * @return true not support, false support.
	 */
	virtual bool IsAnnotationDisable() = 0;
	/**
	 * @brief Display annotation toolbar.
	 * @param viewtype Specify which view to display the toolbar. For more details, see \link SDKViewType \endlink enum.
	 * @param left Specify X-axis coordinate of the upper-left corner for the toolbar.
	 * @param top Specify Y-axis coordinate of the upper-left corner for the toolbar.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise failed. For extended error information, see \link SDKError \endlink enum.
	 * @note Valid only for ZOOM style user interface mode.
	 */
	virtual SDKError StartAnnotation(SDKViewType viewtype) = 0;
	/**
	 * @brief Close the current annotation toolbar.
	 * @param viewtype Specify which view to close the toolbar. For more details, see \link SDKViewType \endlink enum.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise failed. For extended error information, see \link SDKError \endlink enum.
	 * @note Valid only for ZOOM style user interface mode.
	 */
	virtual SDKError StopAnnotation(SDKViewType viewtype) = 0;
	/**
	 * @brief Choose annotation tool.
	 * @param viewtype Specify which view to display the toolbar. For more details, see \link SDKViewType \endlink enum.
	 * @param type Specify the annotation tool to be used. For the definitions of the tools, see \link AnnotationToolType \endlink enum.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	 * @note Valid only for ZOOM style user interface mode. The tool type ANNOTOOL_PICKER and ANNOTOOL_SPOTLIGHT are not support for viewer.
	 */
	virtual SDKError SetTool(SDKViewType viewtype, AnnotationToolType type) = 0;
	/**
	 * @brief Clear present annotations. 
	 * @param viewtype Specify on which view to clear the annotations. For more details, see \link SDKViewType \endlink enum.
	 * @param type Specify the ways to clear annotations. For the definitions of the tools, see \link AnnotationClearType \endlink enum.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	 * @note Valid only for ZOOM style user interface mode.
	 */
	virtual SDKError Clear(SDKViewType viewtype, AnnotationClearType type) = 0;

	/**
	 * @brief Set the colors of annotation tools.
	 * @param viewtype The specified color is used on which view. For more details, see \link SDKViewType \endlink enum. 
	 * @param color Specify the color of the annotation tools in RGB format.  
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	 * @note Valid only for ZOOM style user interface mode.
	 */
	virtual SDKError SetColor(SDKViewType viewtype, unsigned long color) = 0;
	/**
	 * @brief Set the line width of annotation tools.
	 * @param viewtype The specified line width is used on which view. For more details, see \link SDKViewType \endlink enum. 
	 * @param lineWidth Specify the line width to annotate.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	 * @note Valid only for ZOOM style user interface mode.
	 */
	virtual SDKError SetLineWidth(SDKViewType viewtype, long lineWidth) = 0;
	
	/**
	 * @brief Undo the last annotation.
	 * @param viewtype Specify on which view to undo the last annotation. For more details, see \link SDKViewType \endlink enum. 
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	 * @note Valid only for ZOOM style user interface mode.
	 */
	virtual SDKError Undo(SDKViewType viewtype) = 0;
	
	/**
	 * @brief Redo the last-undo annotation.
	 * @param viewtype Specify on which view to redo the last-undo annotation. For more details, see \link SDKViewType \endlink enum.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	 * @note Valid only for ZOOM style user interface mode.
	 */
	virtual SDKError Redo(SDKViewType viewtype) = 0;

	/**
	 * @brief Get the controller of annotation tools used in user custom interface mode.
	 * @return If the function succeeds, the return value is the controller of annotation tools used in user custom interface mode. If the function fails, the return value is nullptr.
	 * @note Valid only for user custom interface mode.
	 */
	virtual ICustomizedAnnotationController* GetCustomizedAnnotationController(ICustomizedShareRender* pShareRender = nullptr) = 0;

	/**
	 * @brief Disallow/allow participants to annotate when viewing the sharing content.
	 * @param [in] viewtype: SDK_FIRST_VIEW/SDK_SECOND_VIEW
	 * @param [in] disable TRUE means disabled, FALSE not.
     * @return If the user owns the authority, the return value is SDKErr_Success. Otherwise not. To get extended error information, see \link SDKError \endlink enum.
	 */
	virtual SDKError DisableViewerAnnotation(SDKViewType viewtype, bool bDisable) = 0;

	/**
	 * @brief Determine if viewer's privilege of annotation is disabled.
	 * @param [in] viewtype: SDK_FIRST_VIEW/SDK_SECOND_VIEW
	 * @param [out] bDisabled TRUE means disabled, FALSE not. It validates only when the return value is SDKErr_Success. 
	 * @return If the user owns the authority, the return value is SDKErr_Success. Otherwise not. To get extended error information, see \link SDKError \endlink enum.
	 */
	virtual SDKError IsViewerAnnotationDisabled(SDKViewType viewtype, bool& bDisabled) = 0;

	/**
	 * @brief Determine if it is able to disallow viewer to annotate. 
	 * @param [in] viewtype: SDK_FIRST_VIEW/SDK_SECOND_VIEW
	 * @param [out] bCan TRUE means able, FALSE not. It validates only when the return value is SDKErr_Success. 
	 * @return If the user owns the authority, the return value is SDKErr_Success. Otherwise not. To get extended error information, see \link SDKError \endlink enum.
	 */
	virtual SDKError CanDisableViewerAnnotation(SDKViewType viewtype, bool& bCan) = 0;

	/**
	 * @brief Determine if it is able to annotate(Both the presenter and viewer can call the function).
	 * @param [in] viewtype SDK_FIRST_VIEW/SDK_SECOND_VIEW
	 * @param [out] bCan TRUE means able, FALSE not. It validates only when the return value is SDKErr_Success. 
	 * @return If the user owns the authority, the return value is SDKErr_Success. Otherwise not. To get extended error information, see \link SDKError \endlink enum.
	 */
	virtual SDKError CanDoAnnotation(SDKViewType viewtype,bool& bCan) = 0;

	/**
	 * @brief Determine whether the legal notice for annotation is available
	 * @return True indicates the legal notice for annotation transcript is available. Otherwise False.
	 */
	virtual bool IsAnnotationLegalNoticeAvailable() = 0;

	/**
 	 * @brief Get the annotation legal notices prompt.
	 */
	virtual const zchar_t* getAnnotationLegalNoticesPrompt() = 0;

	/**
	 * @brief the annotation legal notices explained.
	 */
	virtual const zchar_t* getAnnotationLegalNoticesExplained() = 0;

};
END_ZOOM_SDK_NAMESPACE
#endif