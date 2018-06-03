'use strict';

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var A = function A() {
    _classCallCheck(this, A);
};

$(function () {
    var playlist = [];

    var videoWrapper = document.getElementById('video_wrapper');

    function switchVideo() {
        if (videoWrapper.childNodes.length != 0) {
            videoWrapper.removeChild(videoWrapper.childNodes[0]);
        }

        if (playlist.length == 0) {
            preload();
        } else {
            var newVideo = playlist.shift();
            videoWrapper.appendChild(newVideo);
            newVideo.play();
        }
    }

    var playing = false;

    function preloadVideo(url) {
        var video = document.createElement('video');
        playlist.push(video);

        var source = document.createElement('source');
        source.src = url;
        source.type = 'video/mp4';
        video.appendChild(source);
        video.markedAsDownloaded = false;

        video.onplay = function (e) {
            playing = true;
        };

        video.onended = function (e) {
            playing = false;
        };

        video.onprogress = function (e) {
            var video = e.target;
            if (!video.markedAsDownloaded && video.buffered.length > 0 && video.buffered.end(0) == video.duration) {
                //console.log('finished loading video...');
                video.markedAsDownloaded = true;
                preload();
            }

            if (video.buffered.length != 0) {
                //console.log('onprogress', video.buffered.end(0), video.duration);
            } else {
                    //console.log('onprogress');
                }
        };

        video.oncanplaythrough = function (e) {
            //console.log('oncanplaythrough');
        };
    }

    var lock = false;
    function preload() {
        if (!lock) {
            lock = true;
            $.get('/random', function (url) {
                console.log(url);
                preloadVideo(url);
                lock = false;
            });
        }
    }

    setInterval(function () {
        if (videoWrapper.childNodes.length == 0 || !playing) {
            switchVideo();
        }
    }, 10);

    document.addEventListener('keyup', function (e) {
        if (e.keyCode == 32) {
            switchVideo();
            e.preventDefault();
        }
    });

    document.addEventListener('dblclick', function (e) {
        switchVideo();
    });
});

//# sourceMappingURL=main.js.map