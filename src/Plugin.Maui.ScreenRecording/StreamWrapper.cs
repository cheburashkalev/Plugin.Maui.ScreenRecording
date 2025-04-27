using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Plugin.Maui.ScreenRecording
{
    //The video encoder requires a stream that can seek, but it doesn't actually require seeking to work when encoding fragmented MP4.
    //This stream wraps the non-seekable input stream from FFMPEG and lies about being able to seek, so the encoder is happy.
    public class StreamWrapper : Stream
    {
        public StreamWrapper()
        {

        }

        public override bool CanRead => false;

        public override bool CanSeek => true;

        public override bool CanWrite => true;

        public override long Length => 0;

        public override long Position { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

        public override void Flush()
        {

        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            throw new NotImplementedException();
        }

        public override long Seek(long offset, SeekOrigin origin)
        {
            return offset;
        }

        public override void SetLength(long value)
        {
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            Debug.WriteLine($"Buffer count: {FormatSize(count)}");

        }
        private static readonly string[] SizeSuffixes = { "bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
        public static string FormatSize(long byteCount)
        {
            if (byteCount == 0)
                return "0 bytes";

            long absBytes = Math.Abs(byteCount);
            int magnitude = (int)Math.Log(absBytes, 1024);
            double normalizedSize = absBytes / Math.Pow(1024, magnitude);

            string sizeStr = normalizedSize.ToString("0.##");
            return $"{sizeStr} {SizeSuffixes[magnitude]}";
        }
        public override void Close()
        {
            base.Close();
        }
        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
        }
    }
}
