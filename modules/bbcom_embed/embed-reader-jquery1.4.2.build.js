{
    baseUrl: "../../../lingwo_dictionary/js/require",
    paths: {
        lingwo: "../../../lingwo_oss/js",
        lingwo_old: "..",
        bibliobird: "../../../bbcom/modules/js"
    },
    out: "embed-reader-jquery1.4.2.uncompressed.js",
    optimize: "none",

    include: ["jquery-1.4.2","bibliobird/Embed"],
    skipModuleInsertion: true,
    pragmas: {
        jquery: true,
        requireExcludeModify: true,
        requireExcludePlugin: true,
        requireExcludeContext: true
    }
}
