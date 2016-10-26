using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace ServiceState.Common
{
    class MessageBase<T>
    {
        public T Members;

        public MessageBase(T members)
        {
            Members = members;
        }

        public MessageBase()
        {
        }

        // バイト列を構造体に設定する
        public MessageBase(byte[] bytes)
        {
            Members = ServiceState.Common.BitConverterEx.ToStruct<T>(bytes);
        }

        // isEndianConvert==trueの場合、big endian のバイト列を little endian に変換し構造体に設定する
        public MessageBase(byte[] bytes, bool isEndianConvert/*=false*/) // TODO: C#4.0からデフォルト引数対応
        {
            if (isEndianConvert)
            {
                // エンディアン変換あり
                T members = ServiceState.Common.BitConverterEx.ToStruct<T>(bytes);
                Members = ServiceState.Common.BitConverterEx.StructEndianConvert<T>(members);
                return;
            }
            Members = ServiceState.Common.BitConverterEx.ToStruct<T>(bytes);
        }

        // message = "00 01 02 03 ..." の文字列をバイト配列に変換し構造体に設定する
        public MessageBase(string message, bool isEndianConvert/*=false*/)
        {
            MemoryStream ms = new MemoryStream();
            string[] messages = message.Split(' ');
            foreach (String hex in messages)
            {
                byte b1 = Convert.ToByte(hex, 16);
                ms.WriteByte(b1);
            }
            if (isEndianConvert)
            {
                // エンディアン変換あり
                T members = ServiceState.Common.BitConverterEx.ToStruct<T>(ms.ToArray());
                Members = ServiceState.Common.BitConverterEx.StructEndianConvert<T>(members);
                return;
            }
            Members = ServiceState.Common.BitConverterEx.ToStruct<T>(ms.ToArray());
        }

        // 構造体からバイト列に変換する
        public byte[] GetBytes(bool isEndianConvert/*=false*/) // TODO: C#4.0からデフォルト引数対応
        {
            if (isEndianConvert)
            {
                T members = ServiceState.Common.BitConverterEx.StructEndianConvert<T>(Members);
                return ServiceState.Common.BitConverterEx.ToBytes<T>(members);
            }
            return ServiceState.Common.BitConverterEx.ToBytes<T>(Members);
        }

        // 構造体のサイズを返す
        public static int Size()
        {
            return Marshal.SizeOf(typeof(T));
        }
    }
}
