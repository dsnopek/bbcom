{
    baseUrl: "../../../lingwo_dictionary/js/require",
    paths: {
        lingwo: "../../../lingwo_oss/js",
        lingwo_old: "..",
        bibliobird: "../../../bbcom/modules/js"
    },
    out: "embed-reader.uncompressed.js",
    optimize: "none",

    include: ["jquery-stubs","bibliobird/Embed"],
    skipModuleInsertion: true,
    pragmas: {
        requireExcludeModify: true,
        requireExcludePlugin: true,
        requireExcludeContext: true
    }
}
