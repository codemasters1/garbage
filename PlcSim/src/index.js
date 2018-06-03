import {Scene, OrthographicCamera, CylinderGeometry, Mesh, WebGLRenderer, MeshLambertMaterial, Matrix4} from 'three';
// import TrackballControls from 'three-trackballcontrols';

let scene, camera, renderer, material, mesh1, mesh2;

init();
animate();

function init() {
    scene = new Scene();
    camera = new OrthographicCamera(
        -window.innerWidth / 2,
        window.innerWidth / 2,
        window.innerHeight / 2,
        -window.innerHeight / 2,
        -100,
        10000
    );

    // let controls = new TrackballControls(camera);
    // controls.rotateSpeed = 1.0;
    // controls.zoomSpeed = 1.2;
    // controls.panSpeed = 0.8;
    // controls.noZoom = false;
    // controls.noPan = false;
    // controls.staticMoving = true;
    // controls.dynamicDampingFactor = 0.3;
    // controls.keys = [ 65, 83, 68 ];
    // controls.addEventListener( 'change', animate );

    let geometry2 = new CylinderGeometry(5, 5, 100, 16);
    geometry2.applyMatrix(new Matrix4().makeTranslation(0, -50, 0));

    let geometry1 = geometry2.clone();
    geometry1.applyMatrix(new Matrix4().makeRotationZ(Math.PI / 2));

    material = new MeshLambertMaterial({color: 0xff0000, wireframe: false});
    mesh1 = new Mesh(geometry1, material);
    mesh2 = new Mesh(geometry2, material);
    scene.add(mesh1);
    scene.add(mesh2);

    renderer = new WebGLRenderer();
    renderer.setSize(window.innerWidth, window.innerHeight);
    renderer.setClearColor(0xffffff);
    document.body.appendChild(renderer.domElement);

}

function animate() {
    requestAnimationFrame(animate);
    renderer.render(scene, camera);
}

function clamp(min, max, value) {
    return Math.min(Math.max(min, value), max);
}

function test(scale1, scale2) {

    mesh1.scale.x = scale1;
    mesh2.scale.y = scale2;
    mesh2.position.x = 100 * scale1;
}

let scale1 = 1, scale2 = 1;
document.addEventListener('keydown', e => {
    if (e.key === 'ArrowRight') {
        scale1 = clamp(0.0, 1.0, scale1 + 0.1);
    } else if (e.key === 'ArrowDown') {
        scale2 = clamp(0.0, 1.0, scale2 + 0.1);
    } else if (e.key === 'ArrowLeft') {
        scale1 = clamp(0.0, 1.0, scale1 - 0.1);
    } else if (e.key === 'ArrowUp') {
        scale2 = clamp(0.0, 1.0, scale2 - 0.1);
    }
    test(scale1, scale2);
});

test(scale1, scale2);