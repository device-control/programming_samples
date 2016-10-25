using System;
// using System.Collections.Generic;
// using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace ServiceState.Common
{
    static class ReaderIni
    {
        [DllImport("KERNEL32.DLL")]
        private static extern int GetPrivateProfileString(
            string lpApplicationName,
            string lpKeyName,
            string lpDefault,
            StringBuilder lpReturnedstring,
            int nSize,
            string lpFileName);

        public static string GetValue(string path, string section, string key, string defaultKey)
        {
            StringBuilder sb = new StringBuilder(256);
            string fullpath = System.IO.Directory.GetCurrentDirectory() + "\\" + path;
            int res = GetPrivateProfileString(section, key, defaultKey, sb, sb.Capacity, fullpath);
            return sb.ToString();
        }
    }
}
