using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.InteropServices;
//using System.Reflection;

namespace ServiceState.Test
{
    static class BitConverterExTest
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct StructInternal
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)] // 4byte
            public short[] m4;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1)] // 4byte
            public int[] m8;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct StructA
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)] // 4byte
            public byte[] a;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)] // 4byte
            public short[] b;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)] // 16byte = 8 * 2;
            public StructInternal[] c;

            public int d; // 4byte
        }

        public static void Test00()
        {
            Byte[] bytes = new byte[28];
            for (int i = 0; i < bytes.Length; i++)
            {
                bytes[i] = (Byte)i;
            }
            // byte 配列を構造体に変換
            StructA structA = Common.BitConverterEx.ToStruct<StructA>(bytes);
            // データの詰まった構造体のメンバ名を取得
            List<string> members = Common.BitConverterEx.GetMembers<StructA>(structA);
            foreach (string member in members)
            {
                Console.WriteLine(@"structA." + member);
            }
            // 構造体からbyte配列に戻す
            byte[] struct_to_bytes = Common.BitConverterEx.ToBytes<StructA>(structA);
            int index = 0;
            for (; index < struct_to_bytes.Length; index++)
            {
                if (struct_to_bytes[index] != bytes[index]) break; 
            }
            if (index != struct_to_bytes.Length)
            {
                Console.WriteLine("BitConverterEx.ToBytes() 異常");
            }

            // エンディアン変換する
            StructA structA2 = Common.BitConverterEx.ToStruct<StructA>(bytes);
            StructA structA3 = Common.BitConverterEx.ChangeEndianStruct<StructA>(structA2);
            
        }

    }
}
