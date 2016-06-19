using System.Reflection;
using Funq;
using MySql.Data.MySqlClient;
using ServiceStack;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ServiceStack.Api.Swagger;
using ServiceStack.Data;
using ServiceStack.OrmLite;

namespace StkDataSrv
{
	public class AppHost : AppHostHttpListenerSmartPoolBase
	{
		/// <summary>
		/// Default constructor.
		/// Base constructor requires a name and assembly to locate web service classes. 
		/// </summary>
		public AppHost(params Assembly [] dllstoscan)
			: base("bfhq", dllstoscan.ToArray())
		{
			MinThreads = 3;
			MaxThreads = 12;
		}

		public override void Configure(Container container)
		{
			Plugins.Add(new SwaggerFeature());
			{
				var reqlog = new RequestLogsFeature();
				Plugins.Add(reqlog);
			}

			{
				var cfgfile = new IniFile("stksto.ini", null);
				var dbhost = cfgfile.ReadValue("db", "host");
				var dbport = int.Parse(cfgfile.ReadValue("db", "port"));
				var dbuser = cfgfile.ReadValue("db", "user");
				var dbpwd = cfgfile.ReadValue("db", "pwd");
				var dbname = cfgfile.ReadValue("db", "db");

				OrmLiteConfig.DialectProvider = MySqlDialect.Provider;
				//先手工写，以后放到配置文件里去
				var blder = new MySqlConnectionStringBuilder
				{
					Server = dbhost,
					Port = (uint)dbport,
					UserID = dbuser,
					Password = dbpwd,
					Database = dbname,
					OldGuids = true,
					CharacterSet = "utf8",
					Pooling = true
				};
				string ConnStr = blder.GetConnectionString(true);
				container.Register<IDbConnectionFactory>(c =>
				{
					return new OrmLiteConnectionFactory(ConnStr, MySqlDialect.Provider);
				});
			}
			HostConfig conf = new HostConfig();
			//conf.EnableFeatures = Feature.All.Remove(Feature.Csv | Feature.Soap | Feature.Jsv | Feature.Markdown);
			conf.DebugMode = true;
			SetConfig(conf);
		}
	}
}
