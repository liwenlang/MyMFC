Function HttpRequest(url,mode,params)
Dim oauth_http
Set oauth_http=CreateObject("MSXML2.XMLHTTP")
oauth_http.open mode,url,false
If UCase(mode)= "POST" Then
oauth_http.setRequestHeader"Content-Type","application/json"
End If
oauth_http.Send(params)
HttpRequest= oauth_http.responseText
rem MsgBox HttpRequest
Set oauth_http= nothing
End Function

Dim token,url,params
Set objArgs = WScript.Arguments
params= "{""msgtype"":""text"",""text"":{""content"":""服务器28编译成功,模架SVN:" & objArgs(0) & """}}"
url="https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=28fd8efc-d45e-4577-8363-3320d6da52bf"

call (HttpRequest(url,"POST",params))