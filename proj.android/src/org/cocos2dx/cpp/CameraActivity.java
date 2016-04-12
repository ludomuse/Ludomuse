package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.Size;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.content.DialogInterface.OnClickListener;
import android.media.MediaScannerConnection;
import android.media.MediaScannerConnection.MediaScannerConnectionClient;
import android.view.Display;
import android.graphics.PointF;
import android.app.ProgressDialog;
import android.view.KeyEvent;
import android.view.View;
import android.os.AsyncTask;
import android.util.Log;
import java.util.Calendar;
import android.content.DialogInterface;
import com.IHMTEK.LudoMuse.R;

import org.cocos2dx.cpp.jniFacade.JniJavaFacade;
import org.cocos2dx.cpp.jniFacade.WifiDirectFacade;
import org.cocos2dx.cpp.jniFacade.JniCppFacade;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;
import org.cocos2dx.lib.Cocos2dxActivity;

import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.widget.Toast;
import android.widget.ImageView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.lang.Override;
import java.util.List;


public class CameraActivity extends Activity implements OnClickListener {

    private SurfaceView preview = null;
    private SurfaceHolder previewHolder = null;
    private Camera camera = null;
    private boolean inPreview = false;
    ImageView image;
    Bitmap bmp, itembmp;
    static Bitmap mutableBitmap;
    PointF start = new PointF();
    PointF mid = new PointF();
    float oldDist = 1f;
    File imageFileName = null;
    File imageFileFolder = null;
    private MediaScannerConnection msConn;
    Display d;
    int screenhgt, screenwdh;
    ProgressDialog dialog;



    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.cameraview);

        //image = (ImageView) findViewById(R.id.image);
        preview = (SurfaceView) findViewById(R.id.surface);

        previewHolder = preview.getHolder();
        previewHolder.addCallback(surfaceCallback);
        previewHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);

        previewHolder.setFixedSize(getWindow().getWindowManager()
            .getDefaultDisplay().getWidth(), getWindow().getWindowManager()
            .getDefaultDisplay().getHeight());


    }

    @Override
    public void onClick(DialogInterface a_oTnterface, int i) {

    }

    @Override
    public void onResume() {
        super.onResume();
        camera = Camera.open();
    }

    @Override
    public void onPause() {
        if (inPreview) {
            camera.stopPreview();
        }

        camera.release();
        camera = null;
        inPreview = false;
        super.onPause();
    }

    private Camera.Size getBestPreviewSize(int width, int height, Camera.Parameters parameters) {
        Camera.Size result = null;
        for (Camera.Size size: parameters.getSupportedPreviewSizes()) {
            if (size.width <= width && size.height <= height) {
                if (result == null) {
                    result = size;
                } else {
                    int resultArea = result.width * result.height;
                    int newArea = size.width * size.height;
                    if (newArea > resultArea) {
                        result = size;
                    }
                }
            }
        }
        return (result);
    }

    SurfaceHolder.Callback surfaceCallback = new SurfaceHolder.Callback() {
        public void surfaceCreated(SurfaceHolder holder) {
            try {
                camera.setPreviewDisplay(previewHolder);
            } catch (Throwable t) {
                Log.e("PreviewDemo-surfaceCallback",
                    "Exception in setPreviewDisplay()", t);
                Toast.makeText(CameraActivity.this, t.getMessage(), Toast.LENGTH_LONG)
                    .show();
            }
        }

        public void surfaceChanged(SurfaceHolder holder,
        int format, int width,
        int height) {
            Camera.Parameters parameters = camera.getParameters();
            Camera.Size size = getBestPreviewSize(width, height,
            parameters);

            if (size != null) {
                parameters.setPreviewSize(size.width, size.height);
                camera.setParameters(parameters);
                camera.startPreview();
                inPreview = true;
            }
        }

        public void surfaceDestroyed(SurfaceHolder holder) {
            // no-op
        }
    };


    Camera.PictureCallback photoCallback = new Camera.PictureCallback() {
        public void onPictureTaken(final byte[] data, final Camera camera) {
            dialog = ProgressDialog.show(CameraActivity.this, "", "Saving Photo");
            new Thread() {
                public void run() {
                    try {
                        Thread.sleep(1000);
                    } catch (Exception ex) {}
                    onPictureTake(data, camera);
                }
            }.start();
        }
    };



    public void onPictureTake(byte[] data, Camera camera) {

        bmp = BitmapFactory.decodeByteArray(data, 0, data.length);
        mutableBitmap = bmp.copy(Bitmap.Config.ARGB_8888, true);
        savePhoto(mutableBitmap);
        dialog.dismiss();
    }



    class SavePhotoTask extends AsyncTask < byte[], String, String > {@Override
        protected String doInBackground(byte[]...jpeg) {
            File photo = new File(Environment.getExternalStorageDirectory(), "photo.jpg");
            if (photo.exists()) {
                photo.delete();
            }
            try {
                FileOutputStream fos = new FileOutputStream(photo.getPath());
                fos.write(jpeg[0]);
                fos.close();
            } catch (java.io.IOException e) {
                Log.e("PictureDemo", "Exception in photoCallback", e);
            }
            return (null);
        }
    }


    public void savePhoto(Bitmap bmp) {
        imageFileFolder = new File(Environment.getExternalStorageDirectory(), "Rotate");
        imageFileFolder.mkdir();
        FileOutputStream out = null;
        Calendar c = Calendar.getInstance();
        String date = fromInt(c.get(Calendar.MONTH)) + fromInt(c.get(Calendar.DAY_OF_MONTH)) + fromInt(c.get(Calendar.YEAR)) + fromInt(c.get(Calendar.HOUR_OF_DAY)) + fromInt(c.get(Calendar.MINUTE)) + fromInt(c.get(Calendar.SECOND));
        imageFileName = new File(imageFileFolder, date.toString() + ".jpg");
        try {
            out = new FileOutputStream(imageFileName);
            bmp.compress(Bitmap.CompressFormat.JPEG, 100, out);
            out.flush();
            out.close();
            scanPhoto(imageFileName.toString());
            out = null;
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public String fromInt(int val) {
        return String.valueOf(val);
    }

    public void scanPhoto(final String imageFileName) {
        msConn = new MediaScannerConnection(CameraActivity.this, new MediaScannerConnectionClient() {
            public void onMediaScannerConnected() {
                msConn.scanFile(imageFileName, null);
                Log.i("msClient obj  in Photo Utility", "connection established");
            }
            public void onScanCompleted(String path, Uri uri) {
                msConn.disconnect();
                Log.i("msClient obj in Photo Utility", "scan completed");
            }
        });
        msConn.connect();
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_MENU && event.getRepeatCount() == 0) {
            onBack();
        }
        return super.onKeyDown(keyCode, event);
    }

    public void onBack() {
        Log.e("onBack :", "yes");
        camera.takePicture(null, null, photoCallback);
        inPreview = false;
    }

}