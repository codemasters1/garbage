const path = require('path');
const CopyWebpackPlugin = require('copy-webpack-plugin');
const ExtractTextPlugin = require('extract-text-webpack-plugin');

module.exports = {
    entry: {
        bundle: ['./src/index.js'],
        style: ['./src/style.scss']
    },
    output: {
        path: path.resolve(__dirname, 'dist'),
        filename: '[name].js'
    },
    module: {
        rules: [{
            test: /\.scss$/,
            exclude: /(node_modules)/,
            loader: ExtractTextPlugin.extract({
                fallback: 'style-loader',
                use: 'css-loader!sass-loader'
            }),
        }, {
            test: /\.js$/,
            exclude: /(node_modules)/,
            loader: 'babel-loader',
            options: {
                presets: ['es2015']
            }
        }]
    },
    plugins: [
        new CopyWebpackPlugin([
            { from: './src/index.html' }
        ]),
        new ExtractTextPlugin("[name].css")
    ]
};