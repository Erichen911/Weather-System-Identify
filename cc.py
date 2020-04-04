import os
from flask import Flask, request, render_template, Markup
import socket
import json
app = Flask(__name__)

@app.route('/getinfor',methods=['POST'])
def getinfor():
  un = request.form.get("username")
  pw = request.form.get("password")
  if un =="Erichen" and pw == "5078@26e":
    try:
      client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      host = "192.168.1.148"
      port = 2222
      client.connect((host, port))
      sendmsg = "status"
      client.send(sendmsg.encode("utf-8"))
      msg = client.recv(1024)
      # 接收服务端返回的数据，需要解码
      infor = json.loads(msg.decode("utf-8"))
      mem = json.dumps(infor["MEM"])
      gpu0 = json.dumps(infor["GPU0"])
      gpu1 = json.dumps(infor["GPU1"])
      gpu2 = json.dumps(infor["GPU2"])

      return render_template('infor.html', cpu = "hello", memory = mem, gpu0=gpu0,gpu1=gpu1,gpu2=gpu2)
      # return infor
    except:
      return "Connection Wrong"
  else:
    return "Wrong password!Try again!"

@app.route('/getinfor2',methods=['POST'])
def getinfor2():
  try:
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host = "127.0.0.1"
    port = 2222
    client.connect((host, port))
    sendmsg = "status"
    client.send(sendmsg.encode("utf-8"))
    msg = client.recv(1024)
    # 接收服务端返回的数据，需要解码
    infor = json.loads(msg.decode("utf-8"))
    mem = json.dumps(infor["MEM"])
    gpu0 = json.dumps(infor["GPU0"])
    gpu1 = json.dumps(infor["GPU1"])
    gpu2 = json.dumps(infor["GPU2"])

    return render_template('infor.html', cpu = "hello", memory = mem, gpu0=gpu0,gpu1=gpu1,gpu2=gpu2)
    # return infor
  except:
    return "Connection Wrong"

@app.route('/', methods=['GET', 'POST'])
def demo():
  return render_template('Index.html')
  # else:
  #   inputText = request.form.get("input_text")
  #   resText = Markup(formatRes(reverseText(inputText)))
  #   return render_template('index.html', input_text = inputText, res_text = resText)

def formatRes(textList):
  return '<p>' + '</p><p>'.join(textList) + '</p>'


if __name__ == '__main__':
    app.run(host="0.0.0.0", port = 2333)