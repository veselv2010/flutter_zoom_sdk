/**
 * @file meeting_qa_ctrl_interface.h
 * @brief Meeting Service qa Interface
 * @note Valid for both ZOOM style and user custom interface mode.
 */
#ifndef _MEETING_QA_CTRL_INTERFACE_H_
#define _MEETING_QA_CTRL_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/**
 * @brief Enumeration of Q&A connect status.
 * Here are more detailed structural descriptions.
 */
enum QAConnectStatus
{
	/** Connecting. */
	QA_STATUS_CONNECTING = 0,
	/** Connected. */
	QA_STATUS_CONNECTED,
	/** Disconnected. */
	QA_STATUS_DISCONNECTED,
	/** Disconnected because of conflict. */
	QA_STATUS_DISCONNECT_CONFLICT,
};

/**
 * @class IAnswerItem
 * @brief Answer item interface class
 */
class IAnswerItem
{
public:
	virtual ~IAnswerItem(){}
	
	/**
	 * @brief Gets the timestamp of the answer.
	 * @return The timestamp of the answer.
	 */
	virtual time_t  GetTimeStamp() = 0;
	
	/**
	 * @brief Gets the text of the answer.
	 * @return The text of the answer.
	 */
	virtual const zchar_t* GetText() = 0;
	
	/**
	 * @brief Gets the sender's name of the answer.
	 * @return The sender's name of the answer.
	 */
	virtual const zchar_t* GetSenderName() = 0;
	
	/**
	 * @brief Gets the related question's id of the answer.
	 * @return The related question's id of the answer.
	 */
	virtual const zchar_t* GetQuestionID() = 0;
	
	/**
	 * @brief Gets the answer id.
	 * @return The answer id.
	 */
	virtual const zchar_t* GetAnswerID() = 0;
	
	/**
	 * @brief Determines if the answer is private or not.
	 * @return true indicates to the answer is private.
	 */
	virtual bool IsPrivate() = 0;
	
	/**
	 * @brief Determines if the answer is live or not.
	 * @return true indicates to the answer is live.
	 */
	virtual bool IsLiveAnswer() = 0;
	
	/**
	 * @brief Determines whether the answer's sender is the user himself or not.
	 * @return true indicates that the answer's sender is the user himself.
	 */
	virtual bool IsSenderMyself() = 0;
};

/**
 * @class IQAItemInfo
 * @brief Question item interface class
 */
class IQAItemInfo
{
public:
	virtual ~IQAItemInfo() {}

	/**
	 * @brief Gets the timestamp of the question.
	 * @return The timestamp of the question.
	 */
	virtual time_t GetTimeStamp() = 0;
	
	/**
	 * @brief Gets the number of the up_voters of the question.
	 * @return The number of the up_voters of the question.
	 */
	virtual unsigned int GetUpvoteNum() = 0;
	
	/**
	 * @brief Gets the text of the question.
	 * @return The text of the question.
	 */
	virtual const zchar_t* GetText() = 0;
	
	/**
	 * @brief Gets the sender's name of the question.
	 * @return The sender's name of the question.
	 */
	virtual const zchar_t* GetSenderName() = 0;
	
	/**
	 * @brief Gets the question id.
	 * @return The question id.
	 */
	virtual const zchar_t* GetQuestionID() = 0;

	/**
	 * @brief Determines if the question is anonymous.
	 * @return true indicates the question is anonymous. 
	 */
	virtual bool IsAnonymous() = 0;
	
	/**
	 * @brief Determines if the question is marked as answered.
	 * @return true indicates the question is marked as answered.
	 */
	virtual bool IsMarkedAsAnswered() = 0;
	
	/**
	 * @brief Determines if the question is marked as dismissed.
	 * @return true indicates the question is marked as dismissed.
	 */
	virtual bool IsMarkedAsDismissed() = 0;
	
	/**
	 * @brief Determines if the question's sender is the user himself or not.
	 * @return true indicates that the question's sender is the user himself.
	 */
	virtual bool IsSenderMyself() = 0;
	
	/**
	 * @brief Determines if the user himself is an up_voter of the question or not.
	 * @return true indicates the user himself is an up_voter of the question.
	 */
	virtual bool IsMySelfUpvoted() = 0;

	/**
	 * @brief Determines if the question has live answers or not.
	 * @return true indicates the question has live answers.
	 */
	virtual bool HasLiveAnswers() = 0;
	
	/**
	 * @brief Determines if the question has text answers or not.
	 * @return true indicates the question has text answers.
	 */
	virtual bool HasTextAnswers() = 0;
	
	/**
	 * @brief Determines if the user himself is answering the question live or not.
	 * @return true indicates the user himself is answering the question live.
	 */
	virtual bool AmILiveAnswering() = 0;
	
	/**
	 * @brief Gets all the users' names who answers the question live.
	 * @return All the users' names who answers the question live. Separated by commas.
	 */
	virtual const zchar_t* GetLiveAnswerName() = 0;
	
	/**
	 * @brief Determines if the question is being answered live or not.
	 * @return true indicates the question is being answered live.
	 */
	virtual bool IsLiveAnswering() = 0;
	
	/**
	 * @brief Gets the list of all the answers to the question.
	 * @return The list of all the answers to the question.
	 */
	virtual IList<IAnswerItem*>* GetAnswerList() = 0;
};

/**
 * @class IMeetingQAControllerEvent
 * @brief Meeting q&a callback event.
 */
class IMeetingQAControllerEvent
{
public:
	virtual ~IMeetingQAControllerEvent() {}

	/**
	 * @brief Callback event of Q&A connecting status. 
	 * @param connectStatus: The value of Q&A connecting status.
	 */
	virtual void OnQAConnectStatus(QAConnectStatus connectStatus) = 0;
	
	/**
	 * @brief Callback event of adding question.
	 * @param questionID The question id.
	 * @param bSuccess Add question successfully or not.
	 */
	virtual void OnAddQuestion(const zchar_t* questionID, bool bSuccess) = 0;

	/**
	 * @brief Callback event of adding answer.
	 * @param answerID The answer id.
	 * @param bSuccess Add answer successfully or not.
	 */
	virtual void OnAddAnswer(const zchar_t* answerID, bool bSuccess) = 0;

	/**
	 * @brief Callback event of marking question as dismissed.
	 * @param question_id The question id.
	 */
	virtual void OnQuestionMarkedAsDismissed(const zchar_t* question_id) = 0;

	/**
	 * @brief Callback event of reopening question.
	 * @param question_id The question id.
	 */
	virtual void OnReopenQuestion(const zchar_t* question_id) = 0;

	/**
	 * @brief Callback event of receiving question.
	 * @param questionID The question id.
	 */
	virtual void OnReceiveQuestion(const zchar_t* questionID) = 0;

	/**
	 * @brief Callback event of receiving answer.
	 * @param answerID The answer id.
	 */
	virtual void OnReceiveAnswer(const zchar_t* answerID) = 0;

	/**
	 * @brief Callback event of user answering live.
	 * @param questionID The question id.
	 */
	virtual void OnUserLivingReply(const zchar_t* questionID) = 0;
	
	/**
	 * @brief Callback event of end of user answering live. 
	 * @param questionID The question id.
	 */
	virtual void OnUserEndLiving(const zchar_t* questionID) = 0;

	/**
	 * @brief Callback event of voting up question.
	 * @param question_id The question id.
	 * @param order_changed The order of the question in question list is changed or not.
	 */
	virtual void OnUpvoteQuestion(const zchar_t* question_id, bool order_changed) = 0;
	
	/**
	 * @brief Callback event of revoking voting up question.
	 * @param question_id The question id.
	 * @param order_changed The order of the question in question list is changed or not.
	 */
	virtual void OnRevokeUpvoteQuestion(const zchar_t* question_id, bool order_changed) = 0;

	/**
	 * @brief Callback event of deleting question(s).
	 * @param lstQuestionID The list of question ids.
	 */
	virtual void OnDeleteQuestion(IList<const zchar_t*>* lstQuestionID) = 0;

	/**
	 * @brief Callback event of  deleting answer(s).
	 * @param lstAnswerID The list of answer ids.
	 */
	virtual void OnDeleteAnswer(IList<const zchar_t*>* lstAnswerID) = 0;

	/**
	 * @brief Callback event of enabling to ask question anonymously.
	 * @param bEnabled Enbabled or not.
	 */
	virtual void OnAllowAskQuestionAnonymousStatus(bool bEnabled) = 0;
	
	/**
	 * @brief Callback event of enabling attendee to view all questions.
	 * @param bEnabled Enbabled or not.
	 */
	virtual void OnAllowAttendeeViewAllQuestionStatus(bool bEnabled) = 0;

	/**
	 * @brief Callback event of enabling attendee to vote up questions.
	 * @param bEnabled Enbabled or not.
	 */
	virtual void OnAllowAttendeeVoteupQuestionStatus(bool bEnabled) = 0;

	/**
	 * @brief Callback event of enabling attendee to comment questions.
	 * @param bEnabled Enbabled or not.
	 */
	virtual void OnAllowAttendeeCommentQuestionStatus(bool bEnabled) = 0;

	/**
	 * @brief Callback event of refreshing q&a data.
	 */
	virtual void OnRefreshQAData() = 0;

	/**
	 * @brief Callback event of meeting QA feature status changes
	 * @param bEnabled true indicates meeting QA feature is on, otherwise not.
	 */
	virtual void onMeetingQAStatusChanged(bool bEnabled) = 0;

	/**
	 * @brief Notify host/cohost has changed the status of ask question.
	 * @param bEnabled Can ask question or not.
	 */
	virtual void onAllowAskQuestionStatus(bool bEnabled) = 0;
};

/**
 * @class IMeetingQAController
 * @brief Meeting q&a controller interface class.
 */
class IMeetingQAController
{
public:

	/**
	 * @brief Sets the meeting q&a controller callback event handler.
	 * @param pEvent A pointer to the IMeetingQAControllerEvent that receives the meeting q&a event.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetEvent(IMeetingQAControllerEvent* pEvent) = 0;
	
	/** 
	 * @note attendee 
	 */
	 
	/**
	 * @brief The attendee adds a question.
	 * @param questionContent Specifies the content of the question.
	 * @param bAskAnonymous Specifies whether the question is asked anonymously.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError AddQuestion(const zchar_t* questionContent, bool bAskAnonymous) = 0;
	
	/**
	 * @brief Gets the question count of the attendee himself.
	 * @return The question count of the attendee himself.
	 */
	virtual int GetMyQuestionCount() = 0;
	
	/**
	 * @brief Gets the list of all the questions that the attendee himself added.
	 * @return The list of all the questions that the attendee himself added.
	 */
	virtual IList<IQAItemInfo *>* GetMyQuestionList() = 0;
	
	/**
	 * @brief The attendee comments a question.
	 * @param questionID Specifies the question id.
	 * @param commentContent Specifies the content of the comment.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note The comment will be cut off if it is over long.
	 */
	virtual SDKError CommentQuestion(const zchar_t* questionID, const zchar_t* commentContent) = 0;

	/** 
	 * @note host 
	 */
	 
	/**
	 * @brief The host answers the question to the question sender privately.
	 * @param questionID Specifies the question id.
	 * @param answerContent Specifies the content of the answer.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note The answer will be cut off if it is over long.
	 */
	virtual SDKError AnswerQuestionPrivate(const zchar_t* questionID, const zchar_t* answerContent) = 0;
	
	/**
	 * @brief The host answers the question publicly.
	 * @param questionID Specifies the question id.
	 * @param answerContent Specifies the content of the answer.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note The answer will be cut off if it is over long.
	 */
	virtual SDKError AnswerQuestionPublic(const zchar_t* questionID, const zchar_t* answerContent) = 0;
	
	/**
	 * @brief The host dismisses the question.
	 * @param questionID Specifies the question id.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError DismissQuestion(const zchar_t* questionID) = 0;

	/**
	 * @brief The host deletes the question.
	 * @param questionID Specifies the question id.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError DeleteQuestion(const zchar_t* questionID) = 0;

	/**
	 * @brief The host deletes the answerID.
	 * @param answerID Specifies the answer id.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError DeleteAnswer(const zchar_t* answerID) = 0;

	/**
	 * @brief The host reopens the question.
	 * @param questionID Specifies the question id.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ReopenQuestion(const zchar_t* questionID) = 0;
	
	/**
	 * @brief Sets the question can be answered live.
	 * @param questionID Specifies the question id.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError StartLiving(const zchar_t* questionID) = 0;
	
	/**
	 * @brief Sets the question can not be answered live.
	 * @param questionID Specifies the question id.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EndLiving(const zchar_t* questionID) = 0;

	/**
	 * @brief Gets the count of the opened questions.
	 * @return The count of the opened questions.
	 */
	virtual int GetOpenQuestionCount() = 0;
	
	/**
	 * @brief Gets the count of the dismissed questions.
	 * @return The count of the dismissed questions.
	 */
	virtual int GetDismissedQuestionCount() = 0;
	
	/**
	 * @brief Gets the count of the answered questions.
	 * @return The count of the answered questions.
	 */
	virtual int GetAnsweredQuestionCount() = 0;
	
	/**
	 * @brief Gets the list of the opened questions.
	 * @return The list of the opened questions.
	 */
	virtual IList<IQAItemInfo *>* GetOpenQuestionList() = 0;
	
	/**
	 * @brief Gets the list of the dismissed questions.
	 * @return The list of the dismissed questions.
	 */
	virtual IList<IQAItemInfo *>* GetDismissedQuestionList() = 0;
	
	/**
	 * @brief Gets the list of the answered questions.
	 * @return The list of the answered questions.
	 */
	virtual IList<IQAItemInfo *>* GetAnsweredQuestionList() = 0;
	
	/**
	 * @brief Enables or disable to ask question anonymously.
	 * @param bEnable true indicates to enable to ask question anonymously.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EnableAnonymousQuestion(bool bEnable) = 0;

	/**
	 * @brief Enables or disable the attendees to view all the questions.
	 * @param bEnable true indicates to enable the attendees to view all the questions.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EnableAttendeeViewAllQuestion(bool bEnable) = 0;

	/**
	 * @brief Enables or disable to comment question.
	 * @param bEnable true indicates to enable to comment question.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EnableQAComment(bool bEnable) = 0;

	/**
	 * @brief Enables or disable to vote up question.
	 * @param bEnable true indicates to enable to vote up question.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EnableQAVoteup(bool bEnable) = 0;

	/** 
	 * @note attendee & host 
	 */
	 
	/**
	 * @brief Determines if the meeting q&a service is enabled.
	 * @return true indicates the meeting q&a service is enabled.
	 */
	virtual bool IsQAEnabled() = 0;

	/**
	 * @brief Determines if enabled to comment question.
	 * @return true indicates enabled.
	 */
	virtual bool IsQACommentEnabled() = 0; 

	/**
	 * @brief Determines if enabled to vote up question.
	 * @return true indicates enabled.
	 */
	virtual bool IsQAVoteupEnabled() = 0;

	/**
	 * @brief Determines if enabled to ask question anonymously.
	 * @return true indicates enabled.
	 */
	virtual bool IsAskQuestionAnonymouslyEnabled() = 0;

	/**
	 * @brief Determines if the attendee can view all the questions.
	 * @return true indicates the attendee can view all the questions.
	 */
	virtual bool IsAttendeeCanViewAllQuestions()=0;

	/**
	 * @brief Gets the list of all the questions.
	 * @return The list of all the questions.
	 */
	virtual IList<IQAItemInfo *>* GetAllQuestionList() = 0;

	/**
	 * @brief Gets a certain question.
	 * @param questionID Specifies the question id.
	 * @return A pointer to IQAItemInfo.
	 */
	virtual IQAItemInfo* GetQuestion(const zchar_t* questionID) = 0;

	/**
	 * @brief Gets a certain answer.
	 * @param answerID Specifies the answer id.
	 * @return A pointer to IAnswerItem.
	 */
	virtual IAnswerItem* GetAnswer(const zchar_t* answerID) = 0;

	/**
	 * @brief Vote up or revoke voting up the question.
	 * @param questionID Specifies the question id.
	 * @param bVokeup true indicates to vote up, false indicates to revoke voting up.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError VoteupQuestion(const zchar_t* questionID, bool bVokeup) = 0;

	/**
	 * @brief Determines whether the legal notice for QA is available
	 * @return true indicates the legal notice for QA is available. Otherwise false.
	 */
	virtual bool IsQALegalNoticeAvailable() = 0;

	/**
	 * @brief Gets the QA legal notices prompt.
	 */
	virtual const zchar_t* getQALegalNoticesPrompt() = 0;

	/**
	 * @brief Gets the QA legal notices explained.
	 */
	virtual const zchar_t* getQALegalNoticesExplained() = 0;

	/**
	 * @brief Sets to enable/disable meeting QA.
	 * @param bEnable true indicates enabled, false means disabled.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EnableMeetingQAFeature(bool bEnable) = 0;

	/**
	 * @brief Determines if meeting QA is enabled in current meeting.
	 * @return true indicates enabled, otherwise not.
	 */
	virtual bool IsMeetingQAFeatureOn() = 0;

	/**
	 * @brief Sets attendee can ask question.
	 * @param bEnable true indicates attendee can ask question, otherwise not.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EnableAskQuestion(bool bEnable) = 0;

	/**
	 * @brief Determines if the ask question is allowed by the host/co-host.
	 * @return true indicates can ask question, otherwise not.
	 */
	virtual bool IsAskQuestionEnabled() = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif