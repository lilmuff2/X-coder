package lilmuff1.xcoder;

import android.net.Uri;
import android.content.ContentResolver;
import android.content.Context;
import androidx.documentfile.provider.DocumentFile;

public class Utils {
    public static String Uri2path(Context context, String fp) {
        Uri uri = Uri.parse(fp);
        ContentResolver contentResolver = context.getContentResolver();
        DocumentFile documentFile = DocumentFile.fromSingleUri(context, uri);
        String filePath = documentFile.getUri().getPath();
        int colonIndex = filePath.indexOf(":");
        if (colonIndex != -1) {
            return "/sdcard/"+filePath.substring(colonIndex + 1);
        } else {
            return filePath;
        }
    }
}
