VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "IMsoDocProp"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit
'Class retrieves a set of properties from MS Office 97/2000
'documents. Creates an XML string and returns this to the caller

'Class public interface

'Get Hyperlinks in document
'ViewHyperLinks( strFullQDoc as String ) returns string

'Constants
Const EXCEL_DOC_EXT As String = ".xls"
Const WORD_DOC_EXT As String = ".doc"
Const APPNAME As String = "msoProp"
Const EVTLOG_OBJ As String = "COMEvtLogger.AMTSEvtLogger"
Const WORD_OBJ As String = "Word.Application"
Const EXCEL_OBJ As String = "Excel.Application"
Const POWERPT_OBJ As String = "PowerPoint.Application"

'XML tags
Const HLINKS_XML_START_TAG = "<HLinksInfo>"
Const HLINKS_XML_END_TAG = "</HLinksInfo>"
Const HLINKS_DATA_XML_START_TAG = "<HLINK_DATA>"
Const HLINKS_DATA_XML_END_TAG = "</HLINK_DATA>"

'MSO application objects
Dim m_objExcelApp As Excel.Application
Dim m_objWordApp As Word.Application
'Dim m_objAccessApp As Access.Application
'Dim m_objPptApp As PowerPoint.Application
'Specific MSO classes need to access functionality

'For Excel we need a Workbooks object
Dim m_objWkbks As Excel.Workbooks
'For Word we need a Document object
Dim m_objWordDoc As Word.Document
'For Powerpoint we need  a Presentation object
'Dim m_objPresDoc As PowerPoint.Presentation

'Flag whether debugging info is to be provided
Dim m_bDebugging As Boolean
'Flag whether logging is turned on/off
Dim m_bLogging As Boolean
'Flag whether applications should show themselves or be hidden
Dim m_bAppVisible As Boolean
'Error Logging object
Dim m_objEvtLogger As AMTSEvtLogger

Private Sub class_initialize()
On Error Resume Next 'on any errors just continue
    ' Any class init
    m_bAppVisible = False
    m_bLogging = False
        
    'create an event logger instance
    Set m_objEvtLogger = CreateObject(EVTLOG_OBJ)
    
    If Not m_objEvtLogger Is Nothing Then
        'add event source to system registry
        m_bLogging = m_objEvtLogger.AddEventSource(APPNAME)
        Set m_objEvtLogger = Nothing
    End If
    
End Sub

Private Sub class_terminate()
    ' Any class cleanup
    Call CleanupAll
End Sub

Private Sub CleanupAll()
    Call CleanupExcelObjs
    Call CleanupWordObjs
    Call CleanupMiscObjs
End Sub

'Cleanup miscellaneous COM objects
Private Sub CleanupMiscObjs()
    If Not m_objEvtLogger Is Nothing Then
        Set m_objEvtLogger = Nothing
    End If
End Sub

'Cleanup Excel specific object instances
Private Sub CleanupExcelObjs()
    'close workbooks object if it is open
    If Not m_objWkbks Is Nothing Then
        m_objWkbks.Close
        Set m_objWkbks = Nothing 'release obj
    End If
    'close Excel Application if it is open
    If Not m_objExcelApp Is Nothing Then
        m_objExcelApp.Quit
        Set m_objExcelApp = Nothing 'release obj
    End If
End Sub

'Cleanup Word specific object instances
Private Sub CleanupWordObjs()
    'close word document if it is open
    If Not m_objWordDoc Is Nothing Then
        m_objWordDoc.Close
        Set m_objWordDoc = Nothing ' release obj
    End If
    
    'close Word app is if is open
    If Not m_objWordApp Is Nothing Then
        m_objWordApp.Quit
        Set m_objWordApp = Nothing
    End If
End Sub

Private Function GetExcelDocHLinks(strFullQDoc As String) As String
On Error GoTo ERR_GET_EXCEL_DOC_HLINKS
GetExcelDocHLinks = "" 'set return value as empty string for now

'local declarations
Dim i As Integer
Dim nHLinkCount As Integer
Dim x As Integer
Dim nSheetCount As Integer
GetExcelDocHLinks = GetExcelDocHLinks & HLINKS_DATA_XML_START_TAG
    
Set m_objExcelApp = CreateObject(EXCEL_OBJ)

If m_objExcelApp Is Nothing Then
    ReportError ("MS Excel not registered")
    Exit Function
End If

m_objExcelApp.Visible = m_bAppVisible 'set visibility flag
Set m_objWkbks = m_objExcelApp.Workbooks
Call m_objWkbks.Open(strFullQDoc, , Excel.xlReadOnly)
nSheetCount = m_objExcelApp.ActiveWorkbook.Sheets.Count
        
'for each sheet get hyperlinks
For i = 1 To nSheetCount
    With m_objExcelApp.ActiveWorkbook.Sheets(i)
        nHLinkCount = .Hyperlinks.Count
        For x = 1 To nHLinkCount
            GetExcelDocHLinks = GetExcelDocHLinks & .Hyperlinks(x).Address
            GetExcelDocHLinks = GetExcelDocHLinks & .Hyperlinks(x).SubAddress & ";"
        Next
     End With
Next i

'exit function code
EXIT_GET_EXCEL_DOC_HLINKS:
GetExcelDocHLinks = GetExcelDocHLinks & HLINKS_DATA_XML_END_TAG

Call CleanupAll 'free any resources we must
Exit Function 'leave

'error handling code
ERR_GET_EXCEL_DOC_HLINKS:
'report error
ReportError (Err.Description)
'exit function
GoTo EXIT_GET_EXCEL_DOC_HLINKS
End Function

Private Function GetWordDocHLinks(strFullQDoc As String) As String
On Error GoTo ERR_GET_WORD_DOC_HLINKS
GetWordDocHLinks = ""

'local declarations
Dim nHLinkCount As Integer
Dim i As Integer
GetWordDocHLinks = GetWordDocHLinks & HLINKS_DATA_XML_START_TAG

Set m_objWordApp = CreateObject(WORD_OBJ)

If m_objWordApp Is Nothing Then
    ReportError ("MS Word not registered")
    Exit Function
End If

m_objWordApp.Visible = m_bAppVisible 'set visibility
Set m_objWordDoc = m_objWordApp.Documents.Open(strFullQDoc)
nHLinkCount = m_objWordDoc.Hyperlinks.Count

For i = 1 To nHLinkCount
With m_objWordDoc.Hyperlinks(i)
    
    GetWordDocHLinks = GetWordDocHLinks & .Address
    GetWordDocHLinks = GetWordDocHLinks & .SubAddress & ";"
    
End With
Next i

'exit function code
EXIT_GET_WORD_DOC_HLINKS:
GetWordDocHLinks = GetWordDocHLinks & HLINKS_DATA_XML_END_TAG

Call CleanupAll 'free any resources we must
Exit Function 'leave

'error handling code
ERR_GET_WORD_DOC_HLINKS:
'report error
ReportError (Err.Description)
'exit function
GoTo EXIT_GET_WORD_DOC_HLINKS
End Function

Private Function IsExcelDocument(strFullQDoc As String) As Boolean
    IsExcelDocument = False 'set return value as false for now
    
    If Not InStr(1, LCase(strFullQDoc), LCase(EXCEL_DOC_EXT)) = 0 Then
        IsExcelDocument = True
        Exit Function
    End If
  
End Function

Private Function IsSupportedDocument(strFullQDoc As String) As Boolean
    'set return value as False initially
    IsSupportedDocument = False
    
    'is Excel doc?
    If IsExcelDocument(strFullQDoc) Then
        IsSupportedDocument = True
        Exit Function
    End If
    
    'is Word doc?
    If IsWordDocument(strFullQDoc) Then
        IsSupportedDocument = True
        Exit Function
    End If
        
End Function

Private Function IsWordDocument(strFullQDoc As String) As Boolean
    IsWordDocument = False 'set return value as false for now
    
    If Not InStr(1, LCase(strFullQDoc), LCase(WORD_DOC_EXT)) = 0 Then
        IsWordDocument = True
        Exit Function
    End If

End Function

Public Function ViewHyperlinks(ByVal strFullQDoc As String) As String
On Error GoTo ERR_VIEW_HLINKS
'open XML tag
ViewHyperlinks = HLINKS_XML_START_TAG

'if document is supported doc then attempt to process it
If IsSupportedDocument(strFullQDoc) Then
    
    'use Excel to get hyperlinks if any
    If IsExcelDocument(strFullQDoc) Then
        ViewHyperlinks = ViewHyperlinks & GetExcelDocHLinks(strFullQDoc)
    End If
    
    'use Word to get hyperlinks if any
    If IsWordDocument(strFullQDoc) Then
        ViewHyperlinks = ViewHyperlinks & GetWordDocHLinks(strFullQDoc)
    End If
    
End If

'exit function code
EXIT_VIEW_HLINKS:

'close XML tag
ViewHyperlinks = ViewHyperlinks & HLINKS_XML_END_TAG
Call CleanupAll 'free resources
Exit Function 'leave

'Error handling code
ERR_VIEW_HLINKS:
'report error
ReportError (Err.Description)
'exit function
GoTo EXIT_VIEW_HLINKS
End Function

Private Sub ReportError(strMsg As String)
On Error Resume Next

'if message string is empty then exit, nothing todo
If Len(strMsg) = 0 Then
    Exit Sub
End If

If m_bLogging Then
    'create object instance
    Set m_objEvtLogger = CreateObject(EVTLOG_OBJ)
    
    If Not m_objEvtLogger Is Nothing Then
        'report error event
        m_bLogging = m_objEvtLogger.ReportErrorEvent(strMsg, APPNAME)
        'free object
        Set m_objEvtLogger = Nothing
    End If
    
End If

End Sub

