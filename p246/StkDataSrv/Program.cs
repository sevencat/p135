using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StkDataSrv
{
	class Program
	{
		static void Main(string[] args)
		{
			ServiceStack.Logging.LogManager.LogFactory = new ServiceStack.Logging.ConsoleLogFactory();
			ServiceStack.Logging.ILog log = ServiceStack.Logging.LogManager.GetLogger(typeof(Program));
			var appHost = new AppHost(typeof(Program).Assembly);
			appHost.Init();
			int Port = 9090;
			appHost.Start(string.Format("http://*:{0}/", Port));
			while (true)
			{
				System.Threading.Thread.Sleep(1000000);
			}
		}
	}
}
