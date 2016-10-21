using System;
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
        static T ToStruct<T>(byte[] bytes)
        {
            GCHandle gch = GCHandle.Alloc(bytes, GCHandleType.Pinned);
            T result = (T)Marshal.PtrToStructure(gch.AddrOfPinnedObject(), typeof(T));
            gch.Free();
            return result;
        }

        public static List<string> getMembers<T>(T obj)
        {
            Type type = obj.GetType();
            Stack<string> stack = new Stack<string>();
            stack.Push(type.Name);
            List<string> members = new List<string>();
            getMembers(obj, type, stack, members);
            return members;
        }

        public static void getMembers(object obj, Type type, Stack<string> stack, List<string> members)
        {
            FieldInfo[] fields = type.GetFields(BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
            if (type.IsArray)
            {
                Type array_type = type.GetElementType();
                int size = ((Array)obj).Length;
                int i = 0;
                foreach (object tmp in (Array)obj)
                {
                    stack.Push("[" + i + "]");
                    getMembers(tmp, array_type, stack, members);
                    stack.Pop();
                    i++;
                }
                return;
            }
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
                string member_string = "";
                foreach (string e in stack)
                {
                    member_string += "." + e;
                }
                members.Add(member_string);
                return;
            }


            foreach (FieldInfo info in fields)
            {
                stack.Push(info.Name);
                Type fieldType = info.FieldType;
                object tmp = info.GetValue(obj);
                //PropertyInfo propertyInfo = type.GetProperty(info.Name);
                //Array a = (Array)propertyInfo.GetValue(_obj, null);

                //TypeCode code = Type.GetTypeCode(fieldType);
                getMembers(tmp, fieldType, stack, members);
                stack.Pop();
            }
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
