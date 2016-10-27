using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;

namespace ServiceState.Test
{
    static class MessageStructTest
    {
        class DummyStruct : Common.MessageBase<DummyStruct.DummyStruct_>
        {
            [StructLayout(LayoutKind.Sequential, Pack = 1)]
            public struct DummyStruct_
            {
                [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)] // 4byte
                public short[] m_short;
                [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1)] // 4byte
                public int[] m_int;
            }

            public DummyStruct()
            {
                Members.m_short = new short[2];
                Members.m_int = new int[1];
            }

            public DummyStruct(byte[] bytes, bool isEndianConvert/*=false*/)
                : base(bytes, isEndianConvert)
            {
            }
        }

        public static void Test00()
        {
            string message = "00 01 02 03 04 05 06 07";
            MemoryStream ms = new MemoryStream();
            string[] messages = message.Split(' ');
            foreach (String hex in messages)
            {
                byte b1 = Convert.ToByte(hex, 16);
                ms.WriteByte(b1);
            }
            byte[] inbytes = ms.ToArray();

            foreach (bool isConvertEndian in new bool[2] { true, false })
            {
                //Console.WriteLine("MessageStructTest::Test00({0})", isConvertEndian);
                // bytes から struct に変換。エンディアン変換あり
                DummyStruct msg0 = new DummyStruct(inbytes, isConvertEndian);
                // struct から bytes に変換。エンディアン変換あり
                byte[] outbytes = msg0.GetBytes(isConvertEndian);
                if (inbytes.Length != outbytes.Length)
                {
                    // 異常
                    Console.WriteLine("MessageStructTest::Test00[00]({0})異常", isConvertEndian);
                    break;
                }

                for (int i = 0; i < inbytes.Length; i++)
                {
                    if (inbytes[i] != outbytes[i])
                    {
                        // 異常
                        Console.WriteLine("MessageStructTest::Test00[01]({0})異常", isConvertEndian);
                        break;
                    }
                }
            }
        }
    }
}
