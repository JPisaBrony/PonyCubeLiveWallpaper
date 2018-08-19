package com.jp.ogrelivewallpaper;

import android.content.res.AssetManager;
import android.service.wallpaper.WallpaperService;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.os.Handler;

public class Service extends WallpaperService {
    private final Handler handler = new Handler();

    @Override
    public Engine onCreateEngine() {
        return new ServiceEngine();
    }

    private class ServiceEngine extends Engine {
        private boolean visible = true;

        @Override
        public void onCreate(SurfaceHolder surfaceHolder) {
            super.onCreate(surfaceHolder);
        }

        @Override
        public void onDestroy() {
            super.onDestroy();
            nativeDestroy();
        }

        @Override
        public void onVisibilityChanged(boolean v) {
            visible = v;
            if(v) {
                handler.post(runnable);
            } else {
                handler.removeCallbacks(runnable);
            }
        }

        @Override
        public void onSurfaceCreated(SurfaceHolder holder) {
            super.onSurfaceCreated(holder);
            nativeSurfaceCreate(holder.getSurface(), getResources().getAssets());
        }

        @Override
        public void onSurfaceDestroyed(SurfaceHolder holder) {
            super.onSurfaceDestroyed(holder);
        }

        private final Runnable runnable = new Runnable() {
            @Override
            public void run() {
                draw();
            }
        };

        private void draw() {
            handler.removeCallbacks(runnable);
            if(visible) {
                nativeDraw();
                handler.post(runnable);
            }
        }

        private native void nativeSurfaceCreate(Surface surface, AssetManager assetManager);
        private native void nativeDraw();
        private native void nativeDestroy();
    }

    static {
        try {
            System.loadLibrary("native-lib");
        } catch(Exception e) {
            throw e;
        }
    }
}
