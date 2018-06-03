/******/ (function(modules) { // webpackBootstrap
/******/ 	// The module cache
/******/ 	var installedModules = {};
/******/
/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {
/******/
/******/ 		// Check if module is in cache
/******/ 		if(installedModules[moduleId]) {
/******/ 			return installedModules[moduleId].exports;
/******/ 		}
/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = installedModules[moduleId] = {
/******/ 			i: moduleId,
/******/ 			l: false,
/******/ 			exports: {}
/******/ 		};
/******/
/******/ 		// Execute the module function
/******/ 		modules[moduleId].call(module.exports, module, module.exports, __webpack_require__);
/******/
/******/ 		// Flag the module as loaded
/******/ 		module.l = true;
/******/
/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}
/******/
/******/
/******/ 	// expose the modules object (__webpack_modules__)
/******/ 	__webpack_require__.m = modules;
/******/
/******/ 	// expose the module cache
/******/ 	__webpack_require__.c = installedModules;
/******/
/******/ 	// define getter function for harmony exports
/******/ 	__webpack_require__.d = function(exports, name, getter) {
/******/ 		if(!__webpack_require__.o(exports, name)) {
/******/ 			Object.defineProperty(exports, name, {
/******/ 				configurable: false,
/******/ 				enumerable: true,
/******/ 				get: getter
/******/ 			});
/******/ 		}
/******/ 	};
/******/
/******/ 	// getDefaultExport function for compatibility with non-harmony modules
/******/ 	__webpack_require__.n = function(module) {
/******/ 		var getter = module && module.__esModule ?
/******/ 			function getDefault() { return module['default']; } :
/******/ 			function getModuleExports() { return module; };
/******/ 		__webpack_require__.d(getter, 'a', getter);
/******/ 		return getter;
/******/ 	};
/******/
/******/ 	// Object.prototype.hasOwnProperty.call
/******/ 	__webpack_require__.o = function(object, property) { return Object.prototype.hasOwnProperty.call(object, property); };
/******/
/******/ 	// __webpack_public_path__
/******/ 	__webpack_require__.p = "";
/******/
/******/ 	// Load entry module and return exports
/******/ 	return __webpack_require__(__webpack_require__.s = 2);
/******/ })
/************************************************************************/
/******/ ([
/* 0 */,
/* 1 */,
/* 2 */
/***/ (function(module, exports, __webpack_require__) {

module.exports = __webpack_require__(3);


/***/ }),
/* 3 */
/***/ (function(module, exports) {

throw new Error("Module build failed: ReferenceError: window is not defined\n    at C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\style-loader\\index.js!C:\\Users\\Host\\WebstormProjects\\PlcSim\\style.scss:130:9\n    at C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\style-loader\\index.js!C:\\Users\\Host\\WebstormProjects\\PlcSim\\style.scss:119:46\n    at module.exports.module.exports (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\style-loader\\index.js!C:\\Users\\Host\\WebstormProjects\\PlcSim\\style.scss:174:46)\n    at Object.module.exports.stylesInDom (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\style-loader\\index.js!C:\\Users\\Host\\WebstormProjects\\PlcSim\\style.scss:82:36)\n    at __webpack_require__ (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\style-loader\\index.js!C:\\Users\\Host\\WebstormProjects\\PlcSim\\style.scss:21:30)\n    at C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\style-loader\\index.js!C:\\Users\\Host\\WebstormProjects\\PlcSim\\style.scss:64:18\n    at Object.<anonymous> (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\style-loader\\index.js!C:\\Users\\Host\\WebstormProjects\\PlcSim\\style.scss:67:10)\n    at Module._compile (module.js:570:32)\n    at Object.exec (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\webpack\\lib\\NormalModule.js:129:12)\n    at C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\extract-text-webpack-plugin\\dist\\loader.js:131:26\n    at compile (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\webpack\\lib\\Compiler.js:304:11)\n    at applyPluginsAsync.err (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\webpack\\lib\\Compiler.js:514:14)\n    at next (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\tapable\\lib\\Tapable.js:202:11)\n    at Compiler.<anonymous> (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\extract-text-webpack-plugin\\dist\\loader.js:112:7)\n    at next (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\tapable\\lib\\Tapable.js:204:14)\n    at Compiler.<anonymous> (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\webpack\\lib\\CachePlugin.js:78:5)\n    at Compiler.applyPluginsAsyncSeries (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\tapable\\lib\\Tapable.js:206:13)\n    at compilation.seal.err (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\webpack\\lib\\Compiler.js:511:11)\n    at Compilation.applyPluginsAsyncSeries (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\tapable\\lib\\Tapable.js:195:46)\n    at self.applyPluginsAsync.err (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\webpack\\lib\\Compilation.js:671:19)\n    at Compilation.applyPluginsAsyncSeries (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\tapable\\lib\\Tapable.js:195:46)\n    at self.applyPluginsAsync.err (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\webpack\\lib\\Compilation.js:662:11)\n    at Compilation.applyPluginsAsyncSeries (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\tapable\\lib\\Tapable.js:195:46)\n    at self.applyPluginsAsync.err (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\webpack\\lib\\Compilation.js:657:10)\n    at Compilation.applyPluginsAsyncSeries (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\tapable\\lib\\Tapable.js:195:46)\n    at sealPart2 (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\webpack\\lib\\Compilation.js:653:9)\n    at Compilation.applyPluginsAsyncSeries (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\tapable\\lib\\Tapable.js:195:46)\n    at Compilation.seal (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\webpack\\lib\\Compilation.js:596:8)\n    at applyPluginsParallel.err (C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\webpack\\lib\\Compiler.js:508:17)\n    at C:\\Users\\Host\\WebstormProjects\\PlcSim\\node_modules\\tapable\\lib\\Tapable.js:289:11");

/***/ })
/******/ ]);