using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.Runtime.InteropServices;

// 本ファイルは以下のサイトからベースをいただいた。ライセンス的にどうなんだろう。。。
// エンディアンを指定できるBitConverter
// http://schima.hatenablog.com/entry/20110301/1299001406

namespace ServiceState.Common
{
    public enum Endian
    {
        Little, Big
    }
    
    static class BitConverterEx
    {

        public static bool ToBoolean(byte[] value, int startIndex, Endian endian)
        {
            byte[] sub = GetSubArray(value, startIndex, sizeof(bool));
            return BitConverter.ToBoolean(Reverse(sub, endian), 0);
        }

        public static char ToChar(byte[] value, int startIndex, Endian endian)
        {
            byte[] sub = GetSubArray(value, startIndex, sizeof(char));
            return BitConverter.ToChar(Reverse(sub, endian), 0);
        }

        public static short ToInt16(byte[] value, int startIndex, Endian endian)
        {
            byte[] sub = GetSubArray(value, startIndex, sizeof(short));
            return BitConverter.ToInt16(Reverse(sub, endian), 0);
        }

        public static ushort ToUInt16(byte[] value, int startIndex, Endian endian)
        {
            byte[] sub = GetSubArray(value, startIndex, sizeof(ushort));
            return BitConverter.ToUInt16(Reverse(sub, endian), 0);
        }

        public static int ToInt32(byte[] value, int startIndex, Endian endian)
        {
            byte[] sub = GetSubArray(value, startIndex, sizeof(int));
            return BitConverter.ToInt32(Reverse(sub, endian), 0);
        }

        public static uint ToUInt32(byte[] value, int startIndex, Endian endian)
        {
            byte[] sub = GetSubArray(value, startIndex, sizeof(uint));
            return BitConverter.ToUInt32(Reverse(sub, endian), 0);
        }
        
        public static long ToInt64(byte[] value, int startIndex, Endian endian)
        {
            byte[] sub = GetSubArray(value, startIndex, sizeof(long));
            return BitConverter.ToInt64(Reverse(sub, endian), 0);
        }

        public static ulong ToUInt64(byte[] value, int startIndex, Endian endian)
        {
            byte[] sub = GetSubArray(value, startIndex, sizeof(ulong));
            return BitConverter.ToUInt64(Reverse(sub, endian), 0);
        }
 
        public static float ToSingle(byte[] value, int startIndex, Endian endian)
        {
            byte[] sub = GetSubArray(value, startIndex, sizeof(float));
            return BitConverter.ToSingle(Reverse(sub, endian), 0);
        }
 
        public static double ToDouble(byte[] value, int startIndex, Endian endian)
        {
            byte[] sub = GetSubArray(value, startIndex, sizeof(double));
            return BitConverter.ToDouble(Reverse(sub, endian), 0);
        }

        // 単純な構造体なら上手く整形できるが、配列要素があると上手く動作しない感じ
        // 例えば以下のような配列要素がある場合、どうするべきかがわからない
        /*
        using System.Runtime.InteropServices;
        using System.Reflection;
        
        [StructLayout(LayoutKind.Sequential)]
        public unsafe struct TestBuffer
        {
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            //public byte[] A;
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            //public byte[] B;
            public fixed byte A[4];
            public fixed byte B[2];
        }
        byte[] data = new byte[6]{0,1,2,3,4,5};
        TestBuffer testbuffer = BitConverterEx.ToStruct<TestBuffer>(data,0,Little);
        */
        public static T ToStruct<T>(byte[] value, int startIndex, Endian endian)
            where T : struct
        {
            return (T)ToStruct(value, startIndex, endian, typeof(T));
        }

        public static object ToStruct(byte[] value, int startIndex, Endian endian, Type type)
        {
            if (!type.IsValueType)
                throw new ArgumentException();

            // プリミティブ型は専用メソッドへ飛ばす
            TypeCode code = Type.GetTypeCode(type);
            switch (code)
            {
                case TypeCode.Boolean:
                    return ToBoolean(value, startIndex, endian);
                case TypeCode.Byte:
                    return value[startIndex];
                case TypeCode.Char:
                    return ToChar(value, startIndex, endian);
                case TypeCode.Double:
                    return ToDouble(value, startIndex, endian);
                case TypeCode.Int16:
                    return ToInt16(value, startIndex, endian);
                case TypeCode.Int32:
                    return ToInt32(value, startIndex, endian);
                case TypeCode.Int64:
                    return ToInt64(value, startIndex, endian);
                case TypeCode.SByte:
                    return value[startIndex];
                case TypeCode.Single:
                    return ToSingle(value, startIndex, endian);
                case TypeCode.UInt16:
                    return ToUInt16(value, startIndex, endian);
                case TypeCode.UInt32:
                    return ToUInt32(value, startIndex, endian);
                case TypeCode.UInt64:
                    return ToUInt64(value, startIndex, endian);
                default:
                    break; // 多分その他のstructなので以下処理する
            }

            // 構造体の全フィールドを取得
            FieldInfo[] fields = type.GetFields(BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
            // 型情報から新規インスタンスを生成 (返却値)
            object obj = Activator.CreateInstance(type);
            int offset = 0;
            foreach (FieldInfo info in fields)
            {
                // フィールドの値をバイト列から1つ取得し、objの同じフィールドに設定
                Type fieldType = info.FieldType;
                if (!fieldType.IsValueType)
                    throw new InvalidOperationException();
                /*
                int size = fieldType.StructLayoutAttribute.Size;
                if (0 != size)
                {
                    FieldInfo[] array_fields = fieldType.GetFields(BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
                    Type array_type = array_fields[0].FieldType;
                    Array array_list = Array.CreateInstance(array_type, size);
                    // 型情報から新規インスタンスを生成 (返却値)
                    object array_obj = Activator.CreateInstance(array_type);
                    for (int i = 0; i < size; i++)
                    {
                        object array_fieldValue = ToStruct(value, startIndex + offset, endian, array_type);
                        array_list.SetValue(array_fieldValue, i);
                        offset += Marshal.SizeOf(fieldType) / size;
                    }
                    info.SetValue(obj, array_list);
                    continue;
                }
                */
            
                object fieldValue = ToStruct(value, startIndex + offset, endian, fieldType);
                info.SetValue(obj, fieldValue);
                // 次のフィールド値を見るためフィールドのバイトサイズ分進める
                offset += Marshal.SizeOf(fieldType);
            }

            return obj;
        }

        // 構造体からポインタへ
        public static IntPtr ToPtr<T>(T obj)
        {
            IntPtr ptr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(T)));
            Marshal.StructureToPtr(obj, ptr, false);
            return ptr;
        }

        // 構造体からバイト配列へ
        public static byte[] ToBytes<T>(T obj)
        {
            int size = Marshal.SizeOf(typeof(T));
            IntPtr ptr = Marshal.AllocHGlobal(size);
            Marshal.StructureToPtr(obj, ptr, false);

            byte[] bytes = new byte[size];
            Marshal.Copy(ptr, bytes, 0, size);
            Marshal.FreeHGlobal(ptr);
            return bytes;
        }

        // ポインタから構造体へ
        public static T ToStruct<T>(IntPtr ptr)
        {
            return (T)Marshal.PtrToStructure(ptr, typeof(T));
        }

        // バイト配列から構造体へ
        public static T ToStruct<T>(byte[] bytes)
        {
            GCHandle gch = GCHandle.Alloc(bytes, GCHandleType.Pinned);
            T result = (T)Marshal.PtrToStructure(gch.AddrOfPinnedObject(), typeof(T));
            gch.Free();
            return result;
        }

        // 構造体のエンディアンを変換する(big->little,little->big)
        public static T StructEndianConvert<T>(T obj)
        {
            return (T)_ChangeEndianStruct(obj);
        }
        public static object _ChangeEndianStruct(object obj)
        {
            Type type = obj.GetType();
            FieldInfo[] fields = type.GetFields(BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
            TypeCode code = Type.GetTypeCode(type);
            switch (code)
            {
                case TypeCode.Boolean:
                    // 1byte 何もしない
                    return obj;
                case TypeCode.Byte:
                    // 1byte 何もしない
                    return obj;
                case TypeCode.Char:
                    // 1byte 何もしない
                    return obj;
                case TypeCode.Double: // double 8byte
                    return obj;
                case TypeCode.Int16: // 2byte
                    {
                        byte[] bytes = BitConverter.GetBytes((short)obj);
                        bytes = bytes.Reverse().ToArray();
                        return BitConverter.ToInt16(bytes, 0);
                    }
                case TypeCode.Int32: // 4byte
                    {
                        byte[] bytes = BitConverter.GetBytes((int)obj);
                        bytes = bytes.Reverse().ToArray();
                        return BitConverter.ToInt32(bytes, 0);
                    }
                case TypeCode.Int64: // 8byte
                    {
                        byte[] bytes = BitConverter.GetBytes((long)obj);
                        bytes = bytes.Reverse().ToArray();
                        return BitConverter.ToInt64(bytes, 0);
                    }
                case TypeCode.SByte:
                    // 1byte 何もしない
                    return obj;
                case TypeCode.Single: // float 4byte
                    {
                        byte[] bytes = BitConverter.GetBytes((float)obj);
                        bytes = bytes.Reverse().ToArray();
                        return BitConverter.ToSingle(bytes, 0);
                    }
                case TypeCode.UInt16: // 2byte
                    {
                        byte[] bytes = BitConverter.GetBytes((ushort)obj);
                        bytes = bytes.Reverse().ToArray();
                        return BitConverter.ToUInt16(bytes, 0);
                    }
                case TypeCode.UInt32: // 4byte
                    {
                        byte[] bytes = BitConverter.GetBytes((uint)obj);
                        bytes = bytes.Reverse().ToArray();
                        return BitConverter.ToUInt32(bytes, 0);
                    }
                case TypeCode.UInt64: // 8byte
                    {
                        byte[] bytes = BitConverter.GetBytes((ulong)obj);
                        bytes = bytes.Reverse().ToArray();
                        return BitConverter.ToUInt64(bytes, 0);
                    }
                default:
                    break;
            }
            object new_obj = Activator.CreateInstance(type);
            foreach (FieldInfo info in fields)
            {
                Type fieldType = info.FieldType;
                object tmp = info.GetValue(obj);
                // 配列チェック
                if (fieldType.IsArray)
                {
                    Type array_type = fieldType.GetElementType();
                    int size = ((Array)tmp).Length;
                    int index = 0;
                    object new_array = Activator.CreateInstance(fieldType, size); // エンディアン変換後の配列用バッファ生成
                    foreach (object array_tmp in (Array)tmp)
                    {
                        object val = _ChangeEndianStruct(array_tmp);
                        ((Array)new_array).SetValue(val, index); 
                        index++;
                    }
                    info.SetValue(new_obj, new_array); // エンディアン変換後の配列用バッファをnew_objに書き出し
                    continue;
                }
                object value = _ChangeEndianStruct(tmp);
                info.SetValue(new_obj, value); // new_objに書き出し
            }
            return new_obj;
        }
        
        // メンバー名を取得
        public static List<string> GetMembers<T>(T obj)
        {
            Stack<string> stack = new Stack<string>();
            //stack.Push(type.Name);
            List<string> members = new List<string>();
            GetMembers(obj, stack, members);
            return members;
        }

        public static void GetMembers(object obj, Stack<string> stack, List<string> members)
        {
            Type type = obj.GetType();
            FieldInfo[] fields = type.GetFields(BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);

            TypeCode code = Type.GetTypeCode(type);
            if (code == TypeCode.Boolean ||
                code == TypeCode.Byte ||
                code == TypeCode.Char ||
                code == TypeCode.Double ||
                code == TypeCode.Int16 ||
                code == TypeCode.Int32 ||
                code == TypeCode.Int64 ||
                code == TypeCode.SByte ||
                code == TypeCode.Single ||
                code == TypeCode.UInt16 ||
                code == TypeCode.UInt32 ||
                code == TypeCode.UInt64)
            {
                List<string> _members = new List<string>();
                foreach (string e in stack)
                {
                    _members.Add(e);
                }
                _members.Reverse();
                string[] string_members = _members.ToArray();
                string name = string.Join(".", string_members);
                members.Add(name);
                return;
            }


            foreach (FieldInfo info in fields)
            {
                Type fieldType = info.FieldType;
                object tmp = info.GetValue(obj);
                // 配列チェック
                if (fieldType.IsArray)
                {
                    Type array_type = fieldType.GetElementType();
                    int size = ((Array)tmp).Length;
                    int i = 0;
                    foreach (object array_tmp in (Array)tmp)
                    {
                        stack.Push(info.Name + "[" + i + "]");
                        GetMembers(array_tmp, stack, members);
                        stack.Pop();
                        i++;
                    }
                    continue;
                }
                stack.Push(info.Name);
                GetMembers(tmp, stack, members);
                stack.Pop();
            }
        }

        // 文字列の"00 01 02 03..." をバイト配列の{ 0x00, 0x01, 0x02, 0x03...} に変換
        public static byte[] ConvertToBytesFromString(string message)
        {
            MemoryStream ms = new MemoryStream();
            string[] messages = message.Split(' ');
            foreach (String hex in messages)
            {
                byte b1 = Convert.ToByte(hex, 16);
                ms.WriteByte(b1);
            }
            return ms.ToArray();
        }

        private static byte[] GetSubArray(byte[] src, int startIndex, int count)
        {
            byte[] dst = new byte[count];
            Array.Copy(src, startIndex, dst, 0, count);
            return dst;
        }

        private static byte[] Reverse(byte[] bytes, Endian endian)
        {
            if(BitConverter.IsLittleEndian ^ endian == Endian.Little)
                return bytes.Reverse().ToArray();
            else 
                return bytes;
        }
    }
}
