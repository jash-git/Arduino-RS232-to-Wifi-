using System;
using System.Threading;
using System.Net.Sockets;
using System.Text;

namespace csharp_multi_threaded_server_socket
{
    //資料來源 http://csharp.net-informations.com/communications/csharp-multi-threaded-server-socket.htm
    class Program
    {
        static void Main(string[] args)
        {
            TcpListener serverSocket = new TcpListener(8888);
            TcpClient clientSocket = default(TcpClient);
            int counter = 0;

            serverSocket.Start();
            Console.WriteLine(" >> " + "Server Started");

            counter = 0;

            while (true)
            {
                counter += 1;
                clientSocket = serverSocket.AcceptTcpClient();
                Console.WriteLine(" >> " + "Client No:" + Convert.ToString(counter) + " started!");
                handleClinet client = new handleClinet();
                client.startClient(clientSocket, Convert.ToString(counter));
            }

            clientSocket.Close();
            serverSocket.Stop();
            Console.WriteLine(" >> " + "exit");
            Console.ReadLine();
        }
    }

    //Class to handle each client request separatly
    public class handleClinet
    {
        TcpClient clientSocket;
        string clNo;
        public void startClient(TcpClient inClientSocket, string clineNo)
        {
            this.clientSocket = inClientSocket;
            this.clNo = clineNo;
            Thread ctThread = new Thread(doChat);
            ctThread.Start();
        }

        private void doChat()
        {
            int requestCount = 0;
            byte[] bytesFrom = new byte[99999];//clientSocket.ReceiveBufferSize >65535 jash modify at 20161225
            string dataFromClient = null;
            Byte[] sendBytes = null;
            string serverResponse = null;
            string rCount = null;
            requestCount = 0;

            while ((true))
            {
                try
                {
                    requestCount = requestCount + 1;//通訊次數計數器
                    NetworkStream networkStream = clientSocket.GetStream();//將SOCKET資料流轉到讀取串流物件
                    networkStream.Read(bytesFrom, 0, (int)clientSocket.ReceiveBufferSize);//讀取資料
                    dataFromClient = System.Text.Encoding.ASCII.GetString(bytesFrom);
                    dataFromClient = dataFromClient.Substring(0, dataFromClient.IndexOf("$"));//因為clientSocket.ReceiveBufferSize是65536，所以要有偵測結尾符號(結尾符號兩邊自訂)
                    Console.WriteLine(" >> " + "From client-" + clNo +": "+dataFromClient);

                    rCount = Convert.ToString(requestCount);
                    serverResponse = "Server to clinet(" + clNo + ") " + rCount;//SERVER傳送給CLIENT資料
                    sendBytes = Encoding.ASCII.GetBytes(serverResponse);
                    networkStream.Write(sendBytes, 0, sendBytes.Length);//寫出資料
                    networkStream.Flush();
                    Console.WriteLine(" >> " + serverResponse);
                }
                catch (Exception ex)
                {
                    clientSocket.Close();
                    Console.WriteLine(" >> " + "Server to clinet(" + clNo + ") closed..." );
                    break;
                }
            }
        }

    }
}
