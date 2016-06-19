using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace StkDataSrv
{
    /// <summary>
    ///     读写INI文件的类。
    /// </summary>
    public class IniFile
    {
        private Dictionary<string, Dictionary<string, string>> ini =
            new Dictionary<string, Dictionary<string, string>>(StringComparer.InvariantCultureIgnoreCase);

        public void Parse(string file)
        {
            var txt = File.ReadAllText(file);

            var currentSection = new Dictionary<string, string>(StringComparer.InvariantCultureIgnoreCase);

            ini[""] = currentSection;

            foreach (var line in txt.Split(new[] {"\n"}, StringSplitOptions.RemoveEmptyEntries)
                .Where(t => !string.IsNullOrWhiteSpace(t))
                .Select(t => t.Trim()))
            {
                if (line.StartsWith(";"))
                    continue;


                if (line.StartsWith("[") && line.EndsWith("]"))
                {
                    currentSection = new Dictionary<string, string>(StringComparer.InvariantCultureIgnoreCase);
                    ini[line.Substring(1, line.LastIndexOf("]") - 1)] = currentSection;
                    continue;
                }

                var idx = line.IndexOf("=");
                if (idx == -1)
                    currentSection[line] = "";
                else
                    currentSection[line.Substring(0, idx)] = line.Substring(idx + 1);
            }
        }

        public int ReadInt(string section, string key, int defaultvalue = -1)
        {
            try
            {
                return int.Parse(ReadValue(section, key));
            }
            catch (Exception)
            {
                return defaultvalue;
            }
        }

        public string ReadValue(string key)
        {
            return ReadValue(key, "", "");
        }

        public string ReadValue(string section, string key)
        {
            return ReadValue(section, key, "");
        }

        public string ReadValue(string section, string key, string @default)
        {
            if (!ini.ContainsKey(section))
                return @default;

            if (!ini[section].ContainsKey(key))
                return @default;

            return ini[section][key];
        }

        public string[] GetKeys(string section)
        {
            if (!ini.ContainsKey(section))
                return new string[0];

            return ini[section].Keys.ToArray();
        }

        public string[] GetSections()
        {
            return ini.Keys.Where(t => t != "").ToArray();
        }

        /// <summary>
        ///     cINI Constructor
        /// </summary>
        public IniFile(string pIniFilename)
        {
            Parse(pIniFilename);
        }

        public IniFile(string pinifilename, string path)
        {
            var apppath = AppDomain.CurrentDomain.SetupInformation.ApplicationBase;
            var realpath = apppath;
            if (path != null)
            {
                if (path.Length > 2)
                    realpath = path;
            }
            var ls_IniFilename = realpath + "\\" + pinifilename;
            Parse(ls_IniFilename);
        }
    }
}